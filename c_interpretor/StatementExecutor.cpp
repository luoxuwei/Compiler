//
// Created by 罗旭维 on 2021/10/21.
//

#include "StatementExecutor.h"
#include "GrammarInitializer.h"
#include "Value.h"

void * StatementExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    ICodeNode *node;
    Value *returnObj = NULL;
    switch (production) {
        case GrammarInitializer::FOR_OptExpr_Test_EndOptExpr_Statement_TO_Statement:
            //execute OptExpr
            executeChild(root, 0);
            while (isLoopContinute(root, LoopType::FOR)) {
                //execute statment in for body
                executeChild(root, 3);
                //execute EndOptExpr
                executeChild(root, 2);
            }
            break;
        case GrammarInitializer::Return_Semi_TO_Statement:
            isContinueExecution(false);//解释器执行到return语句的时候，在return后面的其他语句都不能再执行了，return可以出现在函数体的任何地方
            break;
        case GrammarInitializer::Return_Expr_Semi_TO_Statement:
            node = executeChild(root, 0);
            returnObj = (Value *) node->getAttribute(ICodeNode::VALUE);
            setReturnObj(returnObj);
            isContinueExecution(false);
            break;
        default:
            executeChildren(root);
            break;
    }

    return root;
}

bool StatementExecutor::isLoopContinute(ICodeNode *codeNode, LoopType type) {
    ICodeNode *ret = NULL;
    if (LoopType::FOR == type) {
        ret = executeChild(codeNode, 1);
        Value *v = (Value *) ret->getAttribute(ICodeNode::VALUE);
        return v->u.i == 1;
    }
    return false;
}