//
// Created by 罗旭维 on 2021/10/4.
//

#include "Production.h"

Production::Production(CTokenType::Token left, int dot, vector<CTokenType::Token> &right): left(left), dotPos(dot), right(right) {
    if (dot >= right.size()) {
        dotPos = right.size();
    }
}

Production Production::doForword() {
    return Production(left, dotPos + 1, right);
}

CTokenType::Token Production::getDotSymbol() {
    if (dotPos >= right.size()) return CTokenType::Token::UNKNOWN_TOKEN;
    return right.at(dotPos);
}

bool Production::operator==(const Production &production) {
    return left == production.left && right == production.right && dotPos == production.dotPos;
}
