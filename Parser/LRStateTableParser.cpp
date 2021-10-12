//
// Created by 罗旭维 on 2021/10/12.
//

#include "LRStateTableParser.h"

LRStateTableParser::LRStateTableParser(CLexer *l) : lexer(l) {
    statusStack.push(0);//stmt
    lexer->advance();
    lexerInput = lexer->token();
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
    //GrammarInitializer里定义的语法推导表达式
    /*
     * 0:  s -> e
     * 1:  e -> e + t
     * 2:  e -> t
     * 3:  t -> t * f
     * 4:  t -> f
     * 5:  f -> ( e )
     * 6:  f -> NUM
     */
    while (true) {
        int action = getAction(statusStack.top(), lexerInput);
        if (INT32_MAX == action) {
            //解析出错
            printf("\nThe input is denied\n");
            return;
        }

        if (action > 0) {
            showCurrentStateInfo(action);
            //shift 操作
            statusStack.push(action);
            text = lexer->lookAheadText();

            parseStack.push(lexerInput);

            if (CTokenType::isTerminal(lexerInput)) {
                lexer->advance();
                lexerInput = lexer->token();
                valueStack.push_back(NULL);
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

            switch (production->getProductionNum()) {
                const char *topAttribute;
                const char *secondAttribute;
                const char *name;
                case 1://e -> e + t
                case 3://t -> t * f
                    topAttribute = valueStack.at(valueStack.size()-1);
                    secondAttribute = valueStack.at(valueStack.size()-3);
                    if (production->getProductionNum() == 1) {
                        printf("%s += %s\n", secondAttribute, topAttribute);
                    } else {
                        printf("%s *= %s\n", secondAttribute, topAttribute);
                    }
                    free_name(topAttribute);
                    attributeForParentNode = secondAttribute;
                    break;
                case 2://e -> t
                case 4://t -> f
                    attributeForParentNode = valueStack.back();
                    break;
                case 5://f -> ( e )
                    attributeForParentNode = valueStack.at(valueStack.size()-2);
                    break;
                case 6://f -> NUM
                    name = new_name();
                    printf("%s = %s\n", name, text.c_str());
                    attributeForParentNode = name;
                    break;
                default:
                    break;
            }

            int rightSize = production->getRight().size();
            while (rightSize > 0) {
                statusStack.pop();
                parseStack.pop();
                valueStack.pop_back();
                rightSize--;
            }
            lexerInput = production->getLeft();
            parseStack.push(lexerInput);
            valueStack.push_back(attributeForParentNode);
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