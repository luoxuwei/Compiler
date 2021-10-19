//
// Created by 罗旭维 on 2021/10/4.
//

#include "ProductionManager.h"
#include "GrammarInitializer.h"

ProductionManager* ProductionManager::instance = NULL;

ProductionManager * ProductionManager::getInstance() {
    if (instance == NULL) {
        instance = new ProductionManager();
    }
    return instance;
}

ProductionManager::ProductionManager() {
    productionMap = GrammarInitializer::getInstance()->getProductionMap();
    firstSetBuilder = new FirstSetBuilder();
}

vector<Production *> * ProductionManager::getProduction(CTokenType::Token left) {
    auto iter = productionMap->find(left);
    if (iter == productionMap->end()) return NULL;
    return iter->second;
}

void ProductionManager::printAllProductions() {
    for (auto iter : *productionMap) {
        for (auto p : (*iter.second)) {
            p->print();
        }
    }
}

void ProductionManager::initProductions() {
    GrammarInitializer::getInstance()->initProductions();
    productionMap = GrammarInitializer::getInstance()->getProductionMap();
    firstSetBuilder->runFirstSets();
    firstSetBuilder->printAllFistSet();
}

Production * ProductionManager::getProductionByIndex(int index) {
    for (auto i : *productionMap) {
        for (auto p : (*i.second)) {
            if (p->getProductionNum() == index) {
                return p;
            }
        }
    }
    return NULL;
}