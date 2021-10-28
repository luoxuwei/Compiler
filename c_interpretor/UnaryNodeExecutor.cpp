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
                root->setAttribute(ICodeNode::VALUE, (void *) new Value(stof(*text)));
            } else {
                root->setAttribute(ICodeNode::VALUE, (void *) new Value(stoi(*text)));
            }
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
            }
            //找到函数执行树头节点
            func = CodeTreeBuilder::getInstance()->getFunctionNodeByName(*funcName);
            if (func != NULL) {
                executor = ExecutorFactory::getInstance()->getExecutor(func);
                executor->Execute(func);
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
        case GrammarInitializer::Unary_StructOP_Name_TO_Unary:
            child = root->getChildren()->at(0);
            text = (string *) root->getAttribute(ICodeNode::TEXT);
            symbol = (Symbol *) child->getAttribute(ICodeNode::SYMBOL);
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
            break;

    }

    return root;
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