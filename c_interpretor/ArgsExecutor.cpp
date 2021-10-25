//
// Created by 罗旭维 on 2021/10/25.
//

#include "ArgsExecutor.h"
#include "GrammarInitializer.h"
#include "Value.h"
#include <vector>

//把函数调用的参数队列，由字符串转变成数值的队列。有了参数队列之后，如何把参数队列设置到函数对应的参数变量中（symbol）？在UnaryExecutor里，会执行ArgsExecutor
//在UnaryExecutor里执行完ArgsExecutor后，从节点里拿到参数队列，在ArgsExecutor里设置，在UnaryExecutor里拿出来存到FunctionArgumentList里
//有了参数队列之后如何把队列里的数值赋值给被调用函数的参数对象？赋值是在FunctDeclExecutor里实现的
void * ArgsExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    vector<Value *> *argList = new vector<Value *>(), *valueList;
    ICodeNode *child = NULL;
    switch (production) {
        case GrammarInitializer::NoCommaExpr_TO_Args:
            child = (ICodeNode *) executeChild(root, 0);//调用子节点把输入参数进行解读，把字符串解读成数字
            argList->push_back((Value *) child->getAttribute(ICodeNode::VALUE));
            break;
        case GrammarInitializer::NoCommaExpr_Comma_Args_TO_Args:
            child = (ICodeNode *) executeChild(root, 0);//NoCommaExpr 调用子节点把输入参数进行解读，把字符串解读成数字
            argList->push_back((Value *) child->getAttribute(ICodeNode::VALUE));

            child = (ICodeNode *) executeChild(root, 1);//Args 调用子节点把输入参数进行解读，把字符串解读成数字
            valueList = ((Value *) child->getAttribute(ICodeNode::VALUE))->u.list;//解读之后把参数队列加入到list之后，然后设置都这个args对应的节点里（就是root）
            argList->insert(argList->end(), valueList->begin(), valueList->end());
            break;
    }
    root->setAttribute(ICodeNode::VALUE, new Value(argList));
    return root;
}