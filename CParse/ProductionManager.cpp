//
// Created by 罗旭维 on 2021/10/4.
//

#include "ProductionManager.h"

ProductionManager* ProductionManager::instance = NULL;

ProductionManager * ProductionManager::getInstance() {
    if (instance == NULL) {
        instance = new ProductionManager();
    }
    return instance;
}

void ProductionManager::initProductions() {
    vector<CTokenType::Token> *right = NULL;
    right = new vector<CTokenType::Token>({CTokenType::Token::UNKNOWN_TOKEN});
    Production *production = new Production(CTokenType::Token::UNKNOWN_TOKEN, 0, *right);
    addProduction(*production);
}

vector<Production *> * ProductionManager::getProduction(CTokenType::Token left) {
    auto iter = productionMap.find(left);
    if (iter == productionMap.end()) return NULL;
    return iter->second;
}

void ProductionManager::addProduction(Production &production) {
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