//
// Created by 罗旭维 on 2021/10/12.
//

#include "LRStateTableParser.h"

LRStateTableParser::LRStateTableParser(CLexer *l) : lexer(l) {
    statusStack.push(0);//stmt
    lexer->advance();
    lexerInput = CTokenType::EXT_DEF_LIST;
    lrStateTable = GrammarStateManager::getInstance()->getLRStateTable();
    showCurrentStateInfo(0);
}

const char * LRStateTableParser::new_name() {
    if (curName >= 8) {
        printf("\nexpression too complicated\n");
        throw 0;
    }
    return names[curName++];
}

void LRStateTableParser::free_name(const char *s) {
    names[--curName] = s;
}

void LRStateTableParser::showCurrentStateInfo(int stateNum) {
    printf("current input is :%s\n", CTokenType::getSymbolStr(lexerInput));
    printf("current state is:\n");
    GrammarStateManager::getInstance()->getGrammarState(stateNum)->print();
}

void LRStateTableParser::parse() {

    while (true) {
        int action = getAction(statusStack.top(), lexerInput);
        if (INT32_MAX == action) {
            //解析出错
            printf("\nThe input is denied\n");
            return;
        }

        if (action > 0) {
//            showCurrentStateInfo(action);
            //shift 操作
            statusStack.push(action);
            text = lexer->lookAheadText();

            parseStack.push(lexerInput);

            if (CTokenType::isTerminal(lexerInput)) {
                printf("Shift for input: %s\n", CTokenType::getSymbolStr(lexerInput));
                lexer->advance();
                lexerInput = lexer->token();
//                valueStack.push_back(NULL);
            } else {
                //输入是非终结符，不往下读取，恢复到上个输入。
                lexerInput = lexer->token();
            }
        } else {
            if (action == 0) {
                printf("\nThe input can be accepted\n");
                return;
            }
            int reduceProduction = -action;
            Production *production = ProductionManager::getInstance()->getProductionByIndex(reduceProduction);
            printf("reduce by production: \n");
            production->print();


            int rightSize = production->getRight().size();
            while (rightSize > 0) {
                statusStack.pop();
                parseStack.pop();
//                valueStack.pop_back();
                rightSize--;
            }
            lexerInput = production->getLeft();
            parseStack.push(lexerInput);
//            valueStack.push_back(attributeForParentNode);
        }
    }

}

int LRStateTableParser::getAction(int currentState, CTokenType::Token currentInput) {
    auto iter = lrStateTable->find(currentState);
    if (iter == lrStateTable->end()) {
        return INT32_MAX;
    }
    return iter->second[currentInput];
}