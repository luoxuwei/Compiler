//
// Created by 罗旭维 on 2021/10/21.
//

#include "BinaryExecutor.h"
#include "GrammarInitializer.h"
#include <string>
#include "Value.h"
#include "ProgramGenerator.h"

void * BinaryExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    long production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    ICodeNode *child;
    Value *v1, *v2;
    string *s1, *s2, *s;
    switch (production) {
        case GrammarInitializer::Uanry_TO_Binary:
            child = root->getChildren()->at(0);
            copyChild(root, child);
            break;
        case GrammarInitializer::Binary_Plus_Binary_TO_Binary:
        case GrammarInitializer::Binary_DivOp_Binary_TO_Binary:
        case GrammarInitializer::Binary_Minus_Binary_TO_Binary:
        case GrammarInitializer::Binary_Start_Binary_TO_Binary:
            //先假设是整形数相加
            v1 = (Value *) root->getChildren()->at(0)->getAttribute(ICodeNode::VALUE);
            v2 = (Value *) root->getChildren()->at(1)->getAttribute(ICodeNode::VALUE);
            if (production == GrammarInitializer::Binary_Plus_Binary_TO_Binary) {
                root->setAttribute(ICodeNode::VALUE,  (void *) new Value(v1->u.i + v2->u.i));
                s1 = (string *) (root->getChildren()->at(0)->getAttribute(ICodeNode::TEXT));
                s2 = (string *) (root->getChildren()->at(1)->getAttribute(ICodeNode::TEXT));
                s = new string();
                s->append(*s1);
                s->append(" plus ");
                s->append(*s2);
                root->setAttribute(ICodeNode::TEXT, s);
                printf("\n%s is %d\n", s->c_str(), (v1->u.i + v2->u.i));
                ProgramGenerator::getInstance()->emit(Instruction::IADD);
            } else if (production == GrammarInitializer::Binary_Minus_Binary_TO_Binary) {
                root->setAttribute(ICodeNode::VALUE,  (void *) new Value(v1->u.i - v2->u.i));
                s1 = (string *) (root->getChildren()->at(0)->getAttribute(ICodeNode::TEXT));
                s2 = (string *) (root->getChildren()->at(1)->getAttribute(ICodeNode::TEXT));
                s = new string();
                s->append(*s1);
                s->append(" minus ");
                s->append(*s2);
                root->setAttribute(ICodeNode::TEXT, s);
                printf("\n%s is %d\n", s->c_str(), (v1->u.i - v2->u.i));
            } else if (production == GrammarInitializer::Binary_Start_Binary_TO_Binary) {
                root->setAttribute(ICodeNode::VALUE,  (void *) new Value(v1->u.i * v2->u.i));
                s1 = (string *) (root->getChildren()->at(0)->getAttribute(ICodeNode::TEXT));
                s2 = (string *) (root->getChildren()->at(1)->getAttribute(ICodeNode::TEXT));
                s->append(*s1);
                s->append(" * ");
                s->append(*s2);
                root->setAttribute(ICodeNode::TEXT, s);
                printf("\n%s is %d\n", s->c_str(), (v1->u.i * v2->u.i));
                ProgramGenerator::getInstance()->emit(Instruction::IMUL);
            } else {
                root->setAttribute(ICodeNode::VALUE,  (void *) new Value(v1->u.i / v2->u.i));
                s1 = (string *) (root->getChildren()->at(0)->getAttribute(ICodeNode::TEXT));
                s2 = (string *) (root->getChildren()->at(1)->getAttribute(ICodeNode::TEXT));
                s = new string();
                s->append(*s1);
                s->append(" is divided by ");
                s->append(*s2);
                root->setAttribute(ICodeNode::TEXT, s);
                printf("\n%s is %d\n", s->c_str(), (v1->u.i / v2->u.i));
            }

            break;
        case GrammarInitializer::Binary_RelOP_Binary_TO_Binray:
            v1 = (Value *) root->getChildren()->at(0)->getAttribute(ICodeNode::VALUE);
            s1 = (string *) root->getChildren()->at(1)->getAttribute(ICodeNode::TEXT);
            v2 = (Value *) root->getChildren()->at(2)->getAttribute(ICodeNode::VALUE);
            if (*s1 == "==") {
                root->setAttribute(ICodeNode::VALUE, new Value(*v1 == *v2? 1:0));
            } else if (*s1 == "<") {
                root->setAttribute(ICodeNode::VALUE, new Value(*v1 < *v2? 1:0));
            } else if (*s1 == "<=") {
                root->setAttribute(ICodeNode::VALUE, new Value(*v1 <= *v2? 1:0));
            } else if (*s1 == ">") {
                root->setAttribute(ICodeNode::VALUE, new Value(*v1 > *v2? 1:0));
            } else if (*s1 == ">=") {
                root->setAttribute(ICodeNode::VALUE, new Value(*v1 >= *v2? 1:0));
            } else if (*s1 == "!=") {
                root->setAttribute(ICodeNode::VALUE, new Value(*v1 == *v2? 0:1));
            }
            break;
    }

    return root;
}