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

using namespace std;
void * UnaryNodeExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    long production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    string *text = NULL;
    Symbol *symbol = NULL;
    void *value = NULL;
    ICodeNode *child = NULL, *func = NULL;
    string *funcName = NULL;
    Executor *executor = NULL;
    vector<Value *> *list = NULL;
    switch (production) {
        bool isFloat;
        int index;
        Declarator *declarator;
        Value *v;
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
            root->setAttribute(ICodeNode::VALUE, symbol->getValue());
            root->setAttribute(ICodeNode::TEXT, symbol->getName());
            break;
        case GrammarInitializer::String_TO_Unary:
            value = root->getAttribute(ICodeNode::TEXT);
//            symbol = (Symbol *) root->getAttribute(ICodeNode::SYMBOL);
            root->setAttribute(ICodeNode::TEXT, value);
//            root->setAttribute(ICodeNode::SYMBOL, symbol);
            break;
        case GrammarInitializer::Unary_LB_Expr_RB_TO_Unary://这一步还不知道是对数组元素取值还是赋值
            child = root->getChildren()->at(0);
            symbol = (Symbol *) child->getAttribute(ICodeNode::SYMBOL);
            child = root->getChildren()->at(1);
            index = ((Value *) child->getAttribute(ICodeNode::VALUE))->u.i;
            declarator = symbol->getDeclarator(Declarator::ARRAY);
            v = declarator->getElement(index);
            root->setAttribute(ICodeNode::VALUE, v);
            root->setAttribute(ICodeNode::SYMBOL, new ArrayValueSetter(symbol, index));
            root->setAttribute(ICodeNode::TEXT, symbol->getName());
            break;
        case GrammarInitializer::Unary_Incop_TO_Unary:
            symbol = (Symbol *) root->getChildren()->at(0)->getAttribute(ICodeNode::SYMBOL);
            v = symbol->getValue();
            ++(*v);
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
            }
            break;

    }

    return root;
}