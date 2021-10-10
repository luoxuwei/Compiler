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
#include "FirstSetBuilder.h"

using namespace std;
class ProductionManager {
private:
    static ProductionManager *instance;
    GrammarInitializer::ProductionMap *productionMap;
    FirstSetBuilder *firstSetBuilder;
    ProductionManager();
    ProductionManager(const ProductionManager& productionManager) {};
public:
    static ProductionManager* getInstance();
    vector<Production*> *getProduction(CTokenType::Token left);
    void printAllProductions();
    void initProductions();
    FirstSetBuilder * getFirstSetBuilder() {return firstSetBuilder;}
};


#endif //CPARSE_PRODUCTIONMANAGER_H
