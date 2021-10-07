//
// Created by 罗旭维 on 2021/10/7.
//

#include "Symbols.h"

Symbols::Symbols(CTokenType::Token symVal, bool nullable, ProductionList *productions) :value(symVal), nullable(nullable), productions(productions) {
    if (CTokenType::isTerminal(symVal)) {
        firstSet.push_back(symVal);
    }
}

void Symbols::addProduction(vector<CTokenType::Token> production) {
    if (find(productions.begin(), productions.end(), production) == productions.end()) {
        productions.push_back(production);
    }
}