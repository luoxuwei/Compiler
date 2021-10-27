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

ICodeNode * CodeTreeBuilder::getFunctionNodeByName(string &name) {
    auto f = funcMap.find(name);
    if (f != funcMap.end()) {
        return f->second;
    }
    return NULL;
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
                assignSymbolToNode(node, text);
            }
            node->setAttribute(ICodeNode::TEXT, (void *) new string(text));
            break;
        case GrammarInitializer::Unary_LP_RP_TO_Unary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::UNARY);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;
        case GrammarInitializer::Unary_LP_ARGS_RP_TO_Unary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::UNARY);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;
        case GrammarInitializer::Unary_Incop_TO_Unary:
        case GrammarInitializer::Unary_DecOp_TO_Unary:
        case GrammarInitializer::LP_Expr_RP_TO_Unary:
            node = ICodeFactory::createICodeNode(CTokenType::Token::UNARY);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
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
        case GrammarInitializer::Binary_DivOp_Binary_TO_Binary:
        case GrammarInitializer::Binary_Minus_Binary_TO_Binary:
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
        case GrammarInitializer::CompountStmt_TO_Statement:
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
        case GrammarInitializer::FOR_OptExpr_Test_EndOptExpr_Statement_TO_Statement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STATEMENT);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
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
        case GrammarInitializer::While_LP_Test_Rp_TO_Statement:
        case GrammarInitializer::Do_Statement_While_Test_To_Statement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STATEMENT);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;
        case GrammarInitializer::Expr_Semi_TO_OptExpr:
        case GrammarInitializer::Semi_TO_OptExpr:
            node = ICodeFactory::createICodeNode(CTokenType::Token::OPT_EXPR);
            if (production == GrammarInitializer::Expr_Semi_TO_OptExpr) {
                child = codeNodeStack.top();
                codeNodeStack.pop();
                node->addChild(child);
            }
            break;
        case GrammarInitializer::Expr_TO_EndOpt:
            node = ICodeFactory::createICodeNode(CTokenType::Token::END_OPT_EXPR);
            child = codeNodeStack.top();
            codeNodeStack.pop();
            node->addChild(child);
            break;
        case GrammarInitializer::LocalDefs_StmtList_TO_CompoundStmt:
            node = ICodeFactory::createICodeNode(CTokenType::Token::COMPOUND_STMT);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;
        case GrammarInitializer::NewName_LP_RP_TO_FunctDecl://无参函数调用或声明
        case GrammarInitializer::NewName_LP_VarList_RP_TO_FunctDecl:
            node = ICodeFactory::createICodeNode(CTokenType::Token::FUNCT_DECL);
            node->addChild(codeNodeStack.top());//当前栈顶是NAME_TO_NewName产生的NewNAME节点，是函数名
            codeNodeStack.pop();
            child = node->getChildren()->at(0);
            functionName = (string *) child->getAttribute(ICodeNode::TEXT);
            symbol = assignSymbolToNode(node, *functionName);
            break;
        case GrammarInitializer::NewName_TO_VarDecl:
            //下面的NAME_TO_NewName如果是接在NewName_LP_RP_TO_FunctDecl后面就是函数名
            //如果是NewName_TO_VarDecl就只是局部变量声明不需要生成节点，
            //我们暂时不处理变量声明语句
            codeNodeStack.pop();
            break;
        case GrammarInitializer::NAME_TO_NewName:
            node = ICodeFactory::createICodeNode(CTokenType::Token::NEW_NAME);
            node->setAttribute(ICodeNode::TEXT, new string(text));
            break;
        case GrammarInitializer::OptSpecifiers_FunctDecl_CompoundStmt_TO_ExtDef:
            node = ICodeFactory::createICodeNode(CTokenType::Token::EXT_DEF);
            node->addChild(codeNodeStack.top());//CompoundStmt会回推到stmt_list，所以这里栈顶是stmt_list
            codeNodeStack.pop();
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            funcMap[*functionName] = node;
            break;
        case GrammarInitializer::NoCommaExpr_TO_Args:
            node = ICodeFactory::createICodeNode(CTokenType::Token::ARGS);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;
        case GrammarInitializer::NoCommaExpr_Comma_Args_TO_Args:
            node = ICodeFactory::createICodeNode(CTokenType::Token::ARGS);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;
        case GrammarInitializer::Return_Semi_TO_Statement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STATEMENT);
            break;
        case GrammarInitializer::Return_Expr_Semi_TO_Statement:
            node = ICodeFactory::createICodeNode(CTokenType::Token::STATEMENT);
            node->addChild(codeNodeStack.top());
            codeNodeStack.pop();
            break;

    }

    if (node != NULL) {
        node->setAttribute(ICodeNode::PRODUCTION, (void *) production);
        codeNodeStack.push(node);
    }
    return node;
}

ICodeNode * CodeTreeBuilder::getCodeTreeRoot() {
    return funcMap["main"];
}

Symbol *CodeTreeBuilder::assignSymbolToNode(ICodeNode *node, string &text) {
    Symbol *symbol = typeSystem->getSymbolByText(text, parser->getCurrentLevel());
    node->setAttribute(ICodeNode::SYMBOL, symbol);
    node->setAttribute(ICodeNode::TEXT, new string(text));
    return symbol;
}