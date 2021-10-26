//
// Created by 罗旭维 on 2021/10/21.
//

#include "NoCommaExprExecutor.h"
#include "Symbol.h"
#include "GrammarInitializer.h"
#include "Value.h"
#include <string>
#include "IValueSetter.h"

void * NoCommaExprExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    long production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    ICodeNode *child;
    string text;
    IValueSetter *symbol;
    Value *value;
    switch (production) {
        case GrammarInitializer::Binary_TO_NoCommaExpr:
            child = root->getChildren()->at(0);
            copyChild(root, child);
            break;
        case GrammarInitializer::NoCommaExpr_Equal_NoCommaExpr_TO_NoCommaExpr:
            child = root->getChildren()->at(0);
            symbol = (IValueSetter *) child->getAttribute(ICodeNode::SYMBOL);
            child = root->getChildren()->at(1);
            value = (Value *) child->getAttribute(ICodeNode::VALUE);
            symbol->setValue(value);
            child = root->getChildren()->at(0);
            child->setAttribute(ICodeNode::VALUE, value);
            copyChild(root, child);
            printf("\nVariable %s is assigned to value of %s\n", ((string *) root->getAttribute(ICodeNode::TEXT))->c_str(), value->toString());
            break;

    }

    return root;
}