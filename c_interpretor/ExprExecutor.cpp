//
// Created by 罗旭维 on 2021/10/21.
//

#include "ExprExecutor.h"
#include "GrammarInitializer.h"


void * ExprExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    switch (production) {
        case GrammarInitializer::NoCommaExpr_TO_Expr:
            copyChild(root, root->getChildren()->at(0));
            break;
    }

    return root;
}