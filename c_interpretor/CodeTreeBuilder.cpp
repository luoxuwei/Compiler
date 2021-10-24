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
    valueStack = parser->getValueStack();
}

ICodeNode * CodeTreeBuilder::buildCodeTree(int production, string text) {
    ICodeNode *node = NULL, *child = NULL, *operatorNode;
    Symbol *symbol = NULL;
    switch (production) {
        case GrammarInitializer::Number_TO_Unary:
        case GrammarInitializer::Name_TO_Unary:
        case GrammarInitializer::String_TO_Unary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::UNARY);
            if (production == GrammarInitializer::Name_TO_Unary) {
                symbol = typeSystem->getSymbolByText(text, parser->getCurrentLevel());
                node->setAttribute(ICodeNode::SYMBOL, symbol);
            }
            node->setAttribute(ICodeNode::TEXT, (void *) new string(text));
            break;
        case GrammarInitializer::Unary_LB_Expr_RB_TO_Unary:
            ////访问或更改数组元素
            node = ICodeFactory::createICodeNode(CTokenType::Token::UNARY);
            node->addChild(codeNodeStack.top());  //EXPR
            codeNodeStack.pop();
            node->addChild(codeNodeStack.top());  //UNARY
            codeNodeStack.pop();
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
        case GrammarInitializer::Binary_RelOP_Binary_TO_Binray:
            node = ICodeFactory::createICodeNode(CTokenType::Token::BINARY);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);

            operatorNode = ICodeFactory::createICodeNode(CTokenType::Token::RELOP);
            operatorNode->setAttribute(ICodeNode::TEXT, new string(*parser->getRelOpereatorText()));
            node->addChild(operatorNode);

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
        case GrammarInitializer::Expr_TO_Test:
            node = ICodeFactory::createICodeNode(CTokenType::Token::TEST);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            break;
        case GrammarInitializer::If_Test_Statement_TO_IFStatement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::IF_STATEMENT);
            child = codeNodeStack.top();//Test
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();//Statement
            codeNodeStack.pop();
            node->addChild(child);
            break;
        case GrammarInitializer::IfElseStatemnt_Else_Statemenet_TO_IfElseStatement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::IF_ELSE_STATEMENT);
            child = codeNodeStack.top();//IfElseStatement:
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();//Statemenet
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

ICodeNode * CodeTreeBuilder::getCodeTreeRoot() {
    return codeNodeStack.top();
}