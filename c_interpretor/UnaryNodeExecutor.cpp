//
// Created by 罗旭维 on 2021/10/21.
//

#include "UnaryNodeExecutor.h"
#include <string>
#include "Symbol.h"
#include "GrammarInitializer.h"

using namespace std;
void * UnaryNodeExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    long production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    string *text;
    Symbol *symbol;
    void *value;
    switch (production) {
        bool isFloat;
        case GrammarInitializer::Number_TO_Unary:
            text = (string *) root->getAttribute(ICodeNode::TEXT);
            isFloat = text->find('.') != string::npos;
            if (isFloat) {
                root->setAttribute(ICodeNode::VALUE, (void *)(int) stof(*text));
            } else {
                root->setAttribute(ICodeNode::VALUE, (void *) stoi(*text));
            }
            break;
        case GrammarInitializer::Name_TO_Unary:
            symbol = (Symbol *) root->getAttribute(ICodeNode::SYMBOL);
            root->setAttribute(ICodeNode::VALUE, symbol->getValue());
            root->setAttribute(ICodeNode::TEXT, (void *) new string(symbol->getName().c_str()));
            break;
        case GrammarInitializer::String_TO_Unary:
            value = root->getAttribute(ICodeNode::TEXT);
            symbol = (Symbol *) root->getAttribute(ICodeNode::SYMBOL);
            root->setAttribute(ICodeNode::TEXT, value);
            root->setAttribute(ICodeNode::SYMBOL, symbol);
            break;

    }

    return root;
}