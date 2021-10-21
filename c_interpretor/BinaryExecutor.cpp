//
// Created by 罗旭维 on 2021/10/21.
//

#include "BinaryExecutor.h"
#include "GrammarInitializer.h"
#include <string>

void * BinaryExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    long production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    ICodeNode *child;
    int v1, v2;
    string *s1, *s2, *s3;
    switch (production) {
        case GrammarInitializer::Uanry_TO_Binary:
            child = root->getChildren()->at(0);
            copyChild(root, child);
            break;
        case GrammarInitializer::Binary_Plus_Binary_TO_Binary:
            root->reverseChildren();
            //先假设是整形数相加
            v1 = (long) root->getChildren()->at(0)->getAttribute(ICodeNode::VALUE);
            v2 = (long) root->getChildren()->at(1)->getAttribute(ICodeNode::VALUE);
            root->setAttribute(ICodeNode::VALUE, (void *) (v1 + v2));
            s1 = (string *) (root->getChildren()->at(0)->getAttribute(ICodeNode::TEXT));
            s2 = (string *) (root->getChildren()->at(1)->getAttribute(ICodeNode::TEXT));
            printf("\nsum of %s and %s is %d\n", s1->c_str(), s2->c_str(), (v1 + v2));
            break;
    }

    return root;
}