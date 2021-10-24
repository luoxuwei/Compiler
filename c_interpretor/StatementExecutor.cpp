//
// Created by 罗旭维 on 2021/10/21.
//

#include "StatementExecutor.h"
#include "GrammarInitializer.h"
#include "Value.h"

void * StatementExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
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