//
// Created by 罗旭维 on 2021/10/20.
//

#include "CodeTreeBuilder.h"
#include "GrammarInitializer.h"
#include "ICodeFactory.h"

CodeTreeBuilder *CodeTreeBuilder::instance = NULL;

CodeTreeBuilder * CodeTreeBuilder::getInstance() {
    if (instance == NULL) {
        instance = new CodeTreeBuilder();
    }
    return instance;
}

CodeTreeBuilder::CodeTreeBuilder() {
    parser = LRStateTableParser::getInstance();
    typeSystem = TypeSystem::getInstance();
}

ICodeNode * CodeTreeBuilder::buildCodeTree(int production, string text) {
    ICodeNode *node = NULL, *child = NULL;
    Symbol *symbol = NULL;
    switch (production) {
        case GrammarInitializer::Number_TO_Unary:
        case GrammarInitializer::Name_TO_Unary:
        case GrammarInitializer::String_TO_Unary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::UNARY);
            if (production == GrammarInitializer::Name_TO_Unary) {
                symbol = getSymbolByText(text);
                node->setAttribute(ICodeNode::SYMBOL, symbol);
            }
            node->setAttribute(ICodeNode::TEXT, (void *) text.c_str());
            break;
        case GrammarInitializer::Uanry_TO_Binary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::BINARY);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->setAttribute(ICodeNode::TEXT, child->getAttribute(ICodeNode::TEXT));
            node->addChild(child);
            break;
        case GrammarInitializer::Binary_TO_NoCommaExpr:
        case GrammarInitializer::NoCommaExpr_Equal_NoCommaExpr_TO_NoCommaExpr:
            node = ICodeFactory::createICodeNode(CTokenType::Token::NO_COMMA_EXPR);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            if (production == GrammarInitializer::NoCommaExpr_Equal_NoCommaExpr_TO_NoCommaExpr) {
                child = codeNodeStack.top();
                codeNodeStack.pop();
                node->addChild(child);
            }
            break;
        case GrammarInitializer::Binary_Plus_Binary_TO_Binary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::BINARY);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            break;
        case GrammarInitializer::NoCommaExpr_TO_Expr:
            node = ICodeFactory::createICodeNode(CTokenType::Token::EXPR);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            break;
        case GrammarInitializer::Expr_Semi_TO_Statement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STATEMENT);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            break;
        case GrammarInitializer::LocalDefs_TO_Statement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STATEMENT);
            break;
        case GrammarInitializer::Statement_TO_StmtList:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STMT_LIST);
            if (codeNodeStack.size() > 0) {
                child = codeNodeStack.top();
                codeNodeStack.pop();
                node->addChild(child);
            }
            break;
        case GrammarInitializer::StmtList_Statement_TO_StmtList:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STMT_LIST);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            break;
    }

    if (node != NULL) {
        node->setAttribute(ICodeNode::PRODUCTION, (void *) production);
        codeNodeStack.push(node);
    }
    return node;
}

Symbol * CodeTreeBuilder::getSymbolByText(string text) {
    vector<Symbol *> *symbolList = typeSystem->getSymbol(text);
    int i = 0;
    int level = parser->getCurrentLevel();
    while (i < symbolList->size()) {
        if (symbolList->at(i)->getLevel() == level) {
            return symbolList->at(i);
        }
    }

    return NULL;
}

ICodeNode * CodeTreeBuilder::getCodeTreeRoot() {
    return codeNodeStack.top();
}