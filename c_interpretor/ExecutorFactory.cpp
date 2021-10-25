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
#include "TestExecutor.h"
#include "IfStatementExecutor.h"
#include "ElseStatementExecutor.h"
#include "OptExprExecutor.h"
#include "EndOptExecutor.h"
#include "InitializerExecutor.h"
#include "CompoundStmtExecutor.h"
#include "FunctDeclExecutor.h"
#include "ExtDefExecutor.h"
#include "ArgsExecutor.h"

ExecutorFactory *ExecutorFactory::instance = NULL;

ExecutorFactory * ExecutorFactory::getInstance() {
    if (instance == NULL) {
        instance = new ExecutorFactory();
    }
    return instance;
}

Executor * ExecutorFactory::getExecutor(ICodeNode *node) {
    if (node == NULL) return NULL;
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
        case CTokenType::Token::TEST:
            return new TestExecutor();
        case CTokenType::Token::IF_STATEMENT:
            return new IfStatementExecutor();
        case CTokenType::Token::IF_ELSE_STATEMENT:
            return new ElseStatementExecutor();
        case CTokenType::Token::OPT_EXPR:
            return new OptExprExecutor();
        case CTokenType::Token::END_OPT_EXPR:
            return new EndOptExecutor();
        case CTokenType::Token::INITIALIZER:
            return new InitializerExecutor();
        case CTokenType::Token::COMPOUND_STMT:
            return new CompoundStmtExecutor();
        case CTokenType::Token::FUNCT_DECL:
            return new FunctDeclExecutor();
        case CTokenType::Token::EXT_DEF:
            return new ExtDefExecutor();
        case CTokenType::Token::ARGS:
            return new ArgsExecutor();
    }

    return NULL;
}