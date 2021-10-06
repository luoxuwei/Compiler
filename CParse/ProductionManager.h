//
// Created by 罗旭维 on 2021/10/4.
//

#ifndef CPARSE_PRODUCTIONMANAGER_H
#define CPARSE_PRODUCTIONMANAGER_H
#include <map>
#include <vector>
#include "CTokenType.h"
#include "Production.h"
#include "GrammarInitializer.h"

using namespace std;
class ProductionManager {
private:
    static ProductionManager *instance;
    GrammarInitializer::ProductionMap *productionMap;
    ProductionManager();
    ProductionManager(const ProductionManager& productionManager) {};
public:
    static ProductionManager* getInstance();
    vector<Production*> *getProduction(CTokenType::Token left);
};


#endif //CPARSE_PRODUCTIONMANAGER_H
