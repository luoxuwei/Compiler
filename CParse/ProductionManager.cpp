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
}

vector<Production *> * ProductionManager::getProduction(CTokenType::Token left) {
    auto iter = productionMap->find(left);
    if (iter == productionMap->end()) return NULL;
    return iter->second;
}
