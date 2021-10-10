//
// Created by 罗旭维 on 2021/10/7.
//

#include "FirstSetBuilder.h"


FirstSetBuilder::FirstSetBuilder() {
    symbolArray = GrammarInitializer::getInstance()->getSymbleArray();
    symbolMap = GrammarInitializer::getInstance()->getSymbolMap();
}

void FirstSetBuilder::runFirstSets() {
    while (runFirstSetPass) {
        runFirstSetPass = false;
        for (auto s : (*symbolArray)) {
            addSymbolFirstSet(s);
        }
    }

    printAllFistSet();
}

void FirstSetBuilder::addSymbolFirstSet(Symbols *symbols) {
    vector<CTokenType::Token> &firstSet = symbols->firstSet;
    if (CTokenType::isTerminal(symbols->value)) {
        if (find(firstSet.begin(), firstSet.end(), symbols->value) == firstSet.end()) {
            firstSet.push_back(symbols->value);
        }
        return;
    }

    Symbols::ProductionList *productionList = symbols->productions;
    for (auto right : *productionList) {
        if (right.size() == 0) continue;
        if (CTokenType::isTerminal(right.at(0)) && (find(firstSet.begin(), firstSet.end(), right.at(0)) == firstSet.end())) {
            runFirstSetPass = true;
            firstSet.push_back(right.at(0));
        } else if (!CTokenType::isTerminal(right.at(0))) {
            int pos = 0;
            Symbols *cur = NULL;
            do {
                cur = symbolMap->find(right.at(pos))->second;
                pos++;
                if (cur->firstSet.size() == 0) continue;
                for (auto s : cur->firstSet) {
                    if (find(firstSet.begin(), firstSet.end(), s) == firstSet.end()) {
                        firstSet.push_back(s);
                        runFirstSetPass = true;
                    }
                }

            } while (pos < right.size() && cur->nullable);
        }
    }

}

bool FirstSetBuilder::isSymbolNullable(CTokenType::Token token) {
    auto iter = symbolMap->find(token);
    if (iter != symbolMap->end()) {
        return iter->second->nullable;
    }
    return false;
}

vector<CTokenType::Token> * FirstSetBuilder::getFirstSet(CTokenType::Token symbol) {
    auto iter = symbolMap->find(symbol);
    if (iter != symbolMap->end()) {
        return &(iter->second->firstSet);
    }
    return NULL;
}

void FirstSetBuilder::printAllFistSet() {
    for (auto s : (*symbolArray)) {
        printFirstSet(s);
    }
}

void FirstSetBuilder::printFirstSet(Symbols *symbols) {
    printf("%s { ", CTokenType::getSymbolStr(symbols->value));
    for (auto s : symbols->firstSet) {
        printf("%s ", CTokenType::getSymbolStr(s));
    }
    printf(" }\n");
    printf("=========\n");
}