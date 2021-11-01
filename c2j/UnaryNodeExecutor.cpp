//
// Created by 罗旭维 on 2021/10/21.
//

#include "UnaryNodeExecutor.h"
#include <string>
#include "Symbol.h"
#include "GrammarInitializer.h"
#include "ArrayValueSetter.h"
#include "CodeTreeBuilder.h"
#include "ExecutorFactory.h"
#include "FunctionArgumentList.h"
#include "ClibCall.h"
#include "PointerValueSetter.h"
#include "MemoryHeap.h"
#include "DirectMemValueSetter.h"
#include "ExecutorBrocasterImpl.h"
#include "ProgramGenerator.h"

using namespace std;
void * UnaryNodeExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    long production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    string *text = NULL;
    Symbol *symbol = NULL, *args = NULL;
    void *value = NULL;
    ICodeNode *child = NULL, *func = NULL;
    string *funcName = NULL;
    Executor *executor = NULL;
    vector<Value *> *list = NULL;
    Value::Buffer buffer(0, 0, -1);
    switch (production) {
        bool isFloat;
        int index, offset;
        Declarator *declarator;
        Value *v;
        char *content;

        case GrammarInitializer::Number_TO_Unary:
            text = (string *) root->getAttribute(ICodeNode::TEXT);
            isFloat = text->find('.') != string::npos;
            if (isFloat) {
                v = new Value(stof(*text));
                root->setAttribute(ICodeNode::VALUE, (void *) v);
            } else {
                v = new Value(stoi(*text));
                root->setAttribute(ICodeNode::VALUE, (void *) v);
            }
            ProgramGenerator::getInstance()->emit(Instruction::SIPUSH, v->toString());
            break;
        case GrammarInitializer::Name_TO_Unary:
            symbol = (Symbol *) root->getAttribute(ICodeNode::SYMBOL);
            if (symbol != NULL) {
                root->setAttribute(ICodeNode::VALUE, symbol->getValue());
                root->setAttribute(ICodeNode::TEXT, symbol->getName());
            }
            break;
        case GrammarInitializer::String_TO_Unary:
            text = (string *)root->getAttribute(ICodeNode::TEXT);
//            symbol = (Symbol *) root->getAttribute(ICodeNode::SYMBOL);
            root->setAttribute(ICodeNode::VALUE, new Value(*text));
//            root->setAttribute(ICodeNode::SYMBOL, symbol);
            break;
        case GrammarInitializer::Unary_LB_Expr_RB_TO_Unary://这一步还不知道是对数组元素取值还是赋值
            child = root->getChildren()->at(0);
            symbol = (Symbol *) child->getAttribute(ICodeNode::SYMBOL);
            child = root->getChildren()->at(1);
            index = ((Value *) child->getAttribute(ICodeNode::VALUE))->u.i;
            declarator = symbol->getDeclarator(Declarator::ARRAY);
            if (declarator != NULL) {
                v = declarator->getElement(index);
                root->setAttribute(ICodeNode::VALUE, v);
                root->setAttribute(ICodeNode::SYMBOL, new ArrayValueSetter(symbol, index));
                root->setAttribute(ICodeNode::TEXT, symbol->getName());
            }
            declarator = symbol->getDeclarator(Declarator::POINTER);
            if (declarator != NULL) {
                setPointerValue(root, symbol, index);
                root->setAttribute(ICodeNode::SYMBOL, new PointerValueSetter(symbol, index));
                root->setAttribute(ICodeNode::TEXT, symbol->getName());
            }
            break;
        case GrammarInitializer::Unary_Incop_TO_Unary:
        case GrammarInitializer::Unary_DecOp_TO_Unary:
            symbol = (Symbol *) root->getChildren()->at(0)->getAttribute(ICodeNode::SYMBOL);
            v = symbol->getValue();
            if (production == GrammarInitializer::Unary_DecOp_TO_Unary) {
                --(*v);
            } else {
                ++(*v);
            }

            break;
        case GrammarInitializer::LP_Expr_RP_TO_Unary:
            child = root->getChildren()->at(0);
            copyChild(root, child);
            break;
        case GrammarInitializer::Start_Unary_TO_Unary:
            child = root->getChildren()->at(0);
            v = (Value *) child->getAttribute(ICodeNode::VALUE);

            MemoryHeap::getMem(v->u.addr, buffer);
            if (buffer.size > 0) {
                offset = v->u.addr - buffer.addr;
                content = (char *) buffer.buf;
                root->setAttribute(ICodeNode::VALUE, new Value((int) content[offset]));
            }
            root->setAttribute(ICodeNode::SYMBOL, new DirectMemValueSetter(v->u.addr));
            break;
        case GrammarInitializer::Unary_LP_RP_TO_Unary://f(),f推出NewName，NewName回推成Unary，左括号(和右括号)推出LP和RP
        case GrammarInitializer::Unary_LP_ARGS_RP_TO_Unary://f(a, b, c)
            //先获得函数名,第一个节点Unary是函数名
            funcName = (string *) root->getChildren()->at(0)->getAttribute(ICodeNode::TEXT);
            if (production == GrammarInitializer::Unary_LP_ARGS_RP_TO_Unary) {
                child = root->getChildren()->at(1);//ARGS
                v = ((Value *) child->getAttribute(ICodeNode::VALUE));
                if (v != NULL) {
                    list = v->u.list;
                }
                FunctionArgumentList::getInstance()->setFuncArgList(list);
                FunctionArgumentList::getInstance()->setFuncArgSymbolList((vector<Symbol *> *) child->getAttribute(ICodeNode::SYMBOL));
            }
            //找到函数执行树头节点
            func = CodeTreeBuilder::getInstance()->getFunctionNodeByName(*funcName);
            if (func != NULL) {
                executor = ExecutorFactory::getInstance()->getExecutor(func);
                ProgramGenerator::getInstance()->setInstructionBuffered(true);
                executor->Execute(func);
                ProgramGenerator::getInstance()->emit(Instruction::RETURN);
                ProgramGenerator::getInstance()->emitDirective(Directive::END_METHOD);
                ProgramGenerator::getInstance()->setInstructionBuffered(false);
                compileFunctionCall(*funcName);
                v = (Value *) func->getAttribute(ICodeNode::VALUE);//return value
                if (v != NULL) {
                    printf("\nfunction call with name %s has return value that is %s\n", funcName->c_str(), v->toString());
                    root->setAttribute(ICodeNode::VALUE, v);
                }
            } else {
                if (ClibCall::getInstance()->isAPICall(*funcName)) {
                    v = ClibCall::getInstance()->invokeAPI(*funcName);
                    root->setAttribute(ICodeNode::VALUE, v);
                }
            }

            break;
        case GrammarInitializer::Unary_StructOP_Name_TO_Unary://访问结构体变量tag.v或tag->v
            child = root->getChildren()->at(0);
            text = (string *) root->getAttribute(ICodeNode::TEXT);
            symbol = (Symbol *) child->getAttribute(ICodeNode::SYMBOL);

            if (isSymbolStructPointer(symbol)) {
                copyBetweenStructAndMem(symbol, false);
            }

            args = symbol->getArgList();
            while (args != NULL) {
                if (*(args->getName()) == *text) {
                    break;
                }
                args = args->getNextSymbol();
            }

            if (args == NULL) {
                printf("access a filed not in struct object!");
                throw 0;
            }
            root->setAttribute(ICodeNode::SYMBOL, args);
            root->setAttribute(ICodeNode::VALUE, args->getValue());

            if (isSymbolStructPointer(symbol)) {
                checkValidPointer(symbol);
                structObjSymbol = symbol;//结构体对应的symbo
                monitorSymbol = args;//被改变的结构体字段对应的Symbol
                ExecutorBrocasterImpl::getInstance()->registerReceiverForAfterExe(this);
            } else {
                structObjSymbol = NULL;
            }
            break;

    }

    return root;
}

void UnaryNodeExecutor::compileFunctionCall(string &funcName) {
    string *declaration = ProgramGenerator::getInstance()->getDeclarationByName(funcName);
    string call(ProgramGenerator::getInstance()->getProgramName());
    call.append("/");
    call.append(*declaration);
    ProgramGenerator::getInstance()->emit(Instruction::INVOKESTATIC, call.c_str());
}

void UnaryNodeExecutor::setPointerValue(ICodeNode *root, Symbol *symbol, int index) {
    Value::Buffer buffer(0, 0, -1);
    Value *value = symbol->getValue();
    MemoryHeap::getMem(value->u.addr, buffer);

    if (buffer.size < 0) return;
    char *content = (char *) buffer.buf;
    if (symbol->getByteSize() == 1) {
        root->setAttribute(ICodeNode::VALUE, new Value((int) content[index]));
    } else {
        int v = 0;
        v = content[index] & 0xff;
        v << 8;
        v = v | (content[index + 1] & 0xff);
        v << 8;
        v = v | (content[index + 2] & 0xff);
        v << 8;
        v = v | (content[index + 3] & 0xff);
        root->setAttribute(ICodeNode::VALUE, new Value(v));
    }
}

bool UnaryNodeExecutor::isSymbolStructPointer(Symbol *symbol) {
    if (symbol->getDeclarator(Declarator::POINTER) != NULL && symbol->getArgList() != NULL) {
        return true;
    }

    return false;
}

bool UnaryNodeExecutor::checkValidPointer(Symbol *symbol) {
    if (symbol->getDeclarator(Declarator::POINTER) != NULL && symbol->getValue() == NULL) {
        printf("Aceess Empty Pointer");
        throw 0;
    }
}

void UnaryNodeExecutor::handleExecutorMessage(ICodeNode *node) {
    int productNum = (long) node->getAttribute(ICodeNode::PRODUCTION);
    if (productNum != GrammarInitializer::NoCommaExpr_Equal_NoCommaExpr_TO_NoCommaExpr) {
        return;
    }
    //由于ICodeNode::SYMBOL字段目前存了三种类型的指针 Symbol IValueSetter vector，所以这里需要确认，指针是不是Symbol 类型
    Symbol *symbol = (Symbol *) node->getAttribute(ICodeNode::SYMBOL);

    if (symbol == NULL) return;
    IValueSetter *valueSetter = dynamic_cast<IValueSetter *>(symbol);
    if (valueSetter != NULL) {
        symbol = valueSetter->getSymbol();
    }

    if (symbol == monitorSymbol) {
        printf("UnaryNodeExecutor receive msg for assign execution");
        copyBetweenStructAndMem(structObjSymbol, true);
    }
}

//如果是通过malloc分配内存给一个结构体的指针这种情况，我们实现的系统里的结构体就出现两个存储结构体字段值的位置，
// 一个是存在字段对应的symbol对象里，一个是MemoryHeap分配的内存里，所以当改变了字段的值，需要同步到memory
//如果结构体指针指向的内存中的数据发生了变动，需要变动的数据从内存同步到Symbol对象中。
void UnaryNodeExecutor::copyBetweenStructAndMem(Symbol *symbol, bool isFromStructToMem) {
    int addr = symbol->getValue()->u.addr;
    Value::Buffer buffer(0, 0, -1);
    MemoryHeap::getMem(addr, buffer);
    stack<Symbol *> stack;
    reverseStructSymbolList(symbol, stack);
    int offset = 0;

    while (!stack.empty()) {
        Symbol *s = stack.top();
        stack.pop();
        if (isFromStructToMem) {
            offset += writeStructVariablesToMem(s, (char *) buffer.buf, offset);
        } else {
            offset += writeMemToStructVariables(s, (char *) buffer.buf, offset);
        }

    }
}

int UnaryNodeExecutor::writeStructVariablesToMem(Symbol *symbol, char *buf, int offset) {
    if (symbol->getArgList() != NULL) {
        //TODO
        return 0;
    }

    int sz = symbol->getByteSize();
    if (symbol->getValue() == NULL) {
        return sz;
    }

    if (symbol->getDeclarator(Declarator::ARRAY) == NULL) {
        Value *value = symbol->getValue();
        buf[offset] = value->u.i & 0xff;
        buf[offset+1] = (value->u.i>>8) & 0xff;
        buf[offset+2] = (value->u.i>>16) & 0xff;
        buf[offset+3] = (value->u.i>>24) & 0xff;
        return sz;
    } else {
        return copyArrayVariableToMem(symbol, buf, offset);
    }
}

int UnaryNodeExecutor::copyArrayVariableToMem(Symbol *symbol, char *buf, int offset) {
    Declarator *declarator = symbol->getDeclarator(Declarator::ARRAY);
    if (declarator == NULL) return 0;

    int sz = symbol->getByteSize();
    int elemCount = declarator->getElementNum();
    for (int i=0; i<elemCount; i++) {
        Value *value = declarator->getElement(i);
        buf[offset] = value->u.i & 0xff;
        buf[offset+1] = (value->u.i>>8) & 0xff;
        buf[offset+2] = (value->u.i>>16) & 0xff;
        buf[offset+3] = (value->u.i>>24) & 0xff;
        offset = offset + 4;
    }

    return sz * elemCount;
}

void UnaryNodeExecutor::reverseStructSymbolList(Symbol *symbol, stack<Symbol *> &stack) {
    Symbol *sym = symbol->getArgList();
    while (sym != NULL) {
        stack.push(sym);
        sym = sym->getNextSymbol();
    }
}

int UnaryNodeExecutor::writeMemToStructVariables(Symbol *symbol, char *mem, int offset) {
    if (symbol->getArgList() != NULL) {
        //struct variable, copy mem to struct recursively
        return 0;
    }

    int sz = symbol->getByteSize();
    if (symbol->getDeclarator(Declarator::ARRAY) == NULL) {
        symbol->setValue(new Value(fromByteArrayToInteger(mem, offset, sz)));
    } else {
        return copyMemToArrayVariable(symbol, mem, offset);
    }

    return sz;
}

int UnaryNodeExecutor::copyMemToArrayVariable(Symbol *symbol, char *mem, int offset) {
    Declarator *declarator = symbol->getDeclarator(Declarator::ARRAY);
    if (declarator == NULL) {
        return 0;
    }

    int sz = symbol->getByteSize();
    int elemCount = declarator->getElementNum();
    int size = 0;
    for (int i=0; i< elemCount; i++) {
        int val = fromByteArrayToInteger(mem, offset+size, sz);
        declarator->addElement(i, new Value(val));
        size = size + sz;
    }
    return size;
}

int UnaryNodeExecutor::fromByteArrayToInteger(char *mem, int offset, int sz) {
    int val = 0;
    switch (sz) {
        case 1:
            val = (mem[offset] & 0xff);
            break;
        case 2:
            val = mem[offset] | (mem[offset+1] << 8);
            break;
        case 4:
            val = mem[offset] | (mem[offset+1] << 8) | (mem[offset+2] << 16) | (mem[offset+3] << 24);
            break;
    }
    return val;
}