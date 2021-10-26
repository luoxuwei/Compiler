//
// Created by 罗旭维 on 2021/10/20.
//

#include "BaseExecutor.h"
#include "ExecutorFactory.h"


bool BaseExecutor::continueExecute = true;
Value *BaseExecutor::returnObj = NULL;

void BaseExecutor::executeChildren(ICodeNode *root) {
    root->reverseChildren();
    vector<ICodeNode *> *child = root->getChildren();
    for (int i = 0; i < child->size(); i++) {
        if (!continueExecute) {
            break;
        }
        ICodeNode *childNode = child->at(i);
        Executor *excutor = ExecutorFactory::getInstance()->getExecutor(childNode);
        if (excutor != NULL) {
            excutor->Execute(childNode);
        } else {
            printf("Not suitable Executor found, node is: %s", childNode->toString());
        }

    }
}

ICodeNode * BaseExecutor::executeChild(ICodeNode *root, int idx) {
    root->reverseChildren();
    vector<ICodeNode *> *child = root->getChildren();
    ICodeNode *childNode = child->at(idx);
    Executor *excutor = ExecutorFactory::getInstance()->getExecutor(childNode);
    if (excutor != NULL) {
        return (ICodeNode *) excutor->Execute(childNode);
    } else {
        printf("Not suitable Executor found, node is: %s", childNode->toString());
        return NULL;
    }
}

void BaseExecutor::copyChild(ICodeNode *root, ICodeNode *child) {
    root->setAttribute(ICodeNode::SYMBOL, child->getAttribute(ICodeNode::SYMBOL));
    root->setAttribute(ICodeNode::VALUE, child->getAttribute(ICodeNode::VALUE));
    root->setAttribute(ICodeNode::TEXT, child->getAttribute(ICodeNode::TEXT));
}

