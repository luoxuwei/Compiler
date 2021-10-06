//
// Created by 罗旭维 on 2021/10/6.
//

#include "GrammarInitializer.h"
GrammarInitializer* GrammarInitializer::instance = NULL;
GrammarInitializer * GrammarInitializer::getInstance() {
    if (instance == NULL) {
        instance = new GrammarInitializer();
    }
    return instance;
}

GrammarInitializer::ProductionMap* GrammarInitializer::getProductionMap() {
    return &productionMap;
}

void GrammarInitializer::initStmtForDebug() {
    vector<CTokenType::Token> *right = NULL;
    right = new vector<CTokenType::Token>({CTokenType::Token::UNKNOWN_TOKEN});
    Production *production = new Production(CTokenType::Token::UNKNOWN_TOKEN, 0, *right);
    addProduction(*production);
}

void GrammarInitializer::addProduction(Production &production) {
    vector<Production *>* productionList = NULL;
    auto iter = productionMap.find(production.getLeft());
    if (iter == productionMap.end()) {
        productionList = new vector<Production *>();
        productionMap[production.getLeft()] = productionList;
    } else {
        productionList = iter->second;
    }

    auto listIter = find_if(productionList->begin(), productionList->end(), ProductionComparator(production));
    if (listIter != productionList->end()) {
        productionList->push_back(&production);
    }
}