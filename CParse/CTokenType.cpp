//
// Created by 罗旭维 on 2021/9/28.
//

#include "CTokenType.h"
bool CTokenType::isTerminal(CTokenType::Token token) {
//    return token >= FIRST_TERMINAL_INDEX && token <= LAST_TERMINAL_INDEX;
    return token == LP || token == RP || token == PLUS || token == STAR || token == SEMI;
}
