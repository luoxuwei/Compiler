//
// Created by 罗旭维 on 2021/10/23.
//

#include "IfStatementExecutor.h"
#include "ICodeNode.h"
#include "Value.h"

void * IfStatementExecutor::Execute(ICodeNode *root) {
    ICodeNode *res = executeChild(root, 0);
    Value *v = (Value *) res->getAttribute(ICodeNode::VALUE);
    copyChild(root, res);
    if (v != NULL && v->u.i == 1) {
        executeChild(root, 1);
    }

    return root;
}