//
// Created by 罗旭维 on 2021/10/24.
//

#include "OptExprExecutor.h"
#include "GrammarInitializer.h"

void * OptExprExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    switch (production) {
        case GrammarInitializer::Semi_TO_OptExpr:
            return root;
        case GrammarInitializer::Expr_Semi_TO_OptExpr:
            return executeChild(root, 0);
            break;
        default:
            return root;
    }
}