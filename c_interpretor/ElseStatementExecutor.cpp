//
// Created by 罗旭维 on 2021/10/23.
//

#include "ElseStatementExecutor.h"
#include "ICodeNode.h"
#include "Value.h"

void * ElseStatementExecutor::Execute(ICodeNode *root) {

    //先执行if 部分
    ICodeNode *res = executeChild(root, 0);
    Value *v = (Value *) res->getAttribute(ICodeNode::VALUE);

    if (v != NULL && v->u.i == 0) {
        //if 部分没有执行，所以执行else部分
        res = executeChild(root, 1);
    }

    copyChild(root, res);

    return root;
}