//
// Created by 罗旭维 on 2021/10/4.
//

#include "Production.h"
#include "ProductionManager.h"

Production::Production(CTokenType::Token left, int dot, vector<CTokenType::Token> &right): left(left), dotPos(dot), right(right) {
    if (dot >= right.size()) {
        dotPos = right.size();
    }
    lookAhead.push_back(CTokenType::Token::SEMI);
}

Production *Production::doForword() {
    Production *production = new Production(left, dotPos + 1, right);
    production->lookAhead.insert(production->lookAhead.end(), lookAhead.begin(), lookAhead.end());
    return production;
}

Production * Production::cloneSelf() {
    Production *production = new Production(left, dotPos, right);
    production->lookAhead = vector<CTokenType::Token>(lookAhead);
    return production;
}

vector<CTokenType::Token> Production::computeFirstSetOfBetaAndC() {
    vector<CTokenType::Token> set;
    vector<CTokenType::Token> *firstSet;
    set.insert(set.end(), lookAhead.begin(), lookAhead.end());
    for (int i=dotPos+1; i<right.size(); i++) {
        firstSet = ProductionManager::getInstance()->getFirstSetBuilder()->getFirstSet(right[i]);
        for (auto token : *firstSet) {
            if (find(set.begin(), set.end(), token) == set.end()) {
                set.push_back(token);
            }
        }

        if (!ProductionManager::getInstance()->getFirstSetBuilder()->isSymbolNullable(right[i])) {
            break;
        }
    }
    return set;
}

void Production::addLookAheadSet(vector<CTokenType::Token> &list) {
    for (auto token : list) {
        if (find(lookAhead.begin(), lookAhead.end(), token) == lookAhead.end()) {
            lookAhead.push_back(token);
        }
    }
}

CTokenType::Token Production::getDotSymbol() {
    if (dotPos >= right.size()) return CTokenType::Token::UNKNOWN_TOKEN;
    return right.at(dotPos);
}

bool Production::operator==(const Production &production) {
    return productionEquals(production) && lookAheadSetComparing(production) == 0;
}

bool Production::coverUp(const Production &production) {
    return productionEquals(production) && lookAheadSetComparing(production) > 0;
}

bool Production::productionEquals(const Production &production) {
    return left == production.left && right == production.right && dotPos == production.dotPos;
}

int Production::lookAheadSetComparing(const Production &production) {
    if (lookAhead.size() > production.lookAhead.size()) {
        return 1;
    }

    if (lookAhead.size() < production.lookAhead.size()) {
        return -1;
    }

    for (int i = 0; i < lookAhead.size(); i++) {
        if (lookAhead.at(i) != production.lookAhead.at(i)) {
            return -1;
        }
    }

    return 0;
}

void Production::print() {
    printf("%s -> ", CTokenType::getSymbolStr(left));
    bool printDot = false;
    for (int i=0; i<right.size(); i++) {
        if (i == dotPos) {
            printf(".");
            printDot = true;
        }
        printf("%s ", CTokenType::getSymbolStr(right.at(i)));
    }
    if (!printDot) {
        printf(".");
    }
    printf("\n");

    printf("look ahead set :{ ");
    for (auto token : lookAhead) {
        printf("%s ", CTokenType::getSymbolStr(token));
    }
    printf("}\n");
}
