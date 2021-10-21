//
// Created by 罗旭维 on 2021/10/20.
//

#include "ExecutorFactory.h"
#include "ICodeNode.h"
#include "CTokenType.h"
#include "UnaryNodeExecutor.h"
#include "BinaryExecutor.h"
#include "NoCommaExprExecutor.h"
#include "ExprExecutor.h"
#include "StatementExecutor.h"
#include "StatementListExecutor.h"

ExecutorFactory *ExecutorFactory::instance = NULL;

ExecutorFactory * ExecutorFactory::getInstance() {
    if (instance == NULL) {
        instance = new ExecutorFactory();
    }
    return instance;
}

Executor * ExecutorFactory::getExecutor(ICodeNode *node) {
    long type = (long) node->getAttribute(ICodeNode::TokenType);
    switch (type) {
        case CTokenType::Token::UNARY:
            return new UnaryNodeExecutor();
        case CTokenType::Token::BINARY:
            return new BinaryExecutor();
        case CTokenType::Token::NO_COMMA_EXPR:
            return new NoCommaExprExecutor();
        case CTokenType::Token::EXPR:
            return new ExprExecutor();
        case CTokenType::Token::STATEMENT:
            return new StatementExecutor();
        case CTokenType::Token::STMT_LIST:
            return new StatementListExecutor();
    }

    return NULL;
}