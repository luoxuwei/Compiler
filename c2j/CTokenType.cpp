//
// Created by 罗旭维 on 2021/9/28.
//

#include "CTokenType.h"
bool CTokenType::isTerminal(CTokenType::Token token) {
    return FIRST_TERMINAL_INDEX <= token && token <= LAST_TERMINAL_INDEX;
}
