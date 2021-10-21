//
// Created by 罗旭维 on 2021/10/20.
//

#include "BaseExecutor.h"
#include "ExecutorFactory.h"


void BaseExecutor::executeChildren(ICodeNode *root) {
    root->reverseChildren();
    vector<ICodeNode *> *child = root->getChildren();
    for (int i = 0; i < child->size(); i++) {
        ICodeNode *childNode = child->at(i);
        Executor *excutor = ExecutorFactory::getInstance()->getExecutor(childNode);
        excutor->Execute(childNode);
    }
}

void BaseExecutor::copyChild(ICodeNode *root, ICodeNode *child) {
    root->setAttribute(ICodeNode::SYMBOL, child->getAttribute(ICodeNode::SYMBOL));
    root->setAttribute(ICodeNode::VALUE, child->getAttribute(ICodeNode::VALUE));
    root->setAttribute(ICodeNode::TEXT, child->getAttribute(ICodeNode::TEXT));
}

