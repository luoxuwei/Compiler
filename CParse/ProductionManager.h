//
// Created by 罗旭维 on 2021/10/4.
//

#ifndef CPARSE_PRODUCTIONMANAGER_H
#define CPARSE_PRODUCTIONMANAGER_H
#include <map>
#include <vector>
#include "CTokenType.h"
#include "Production.h"

using namespace std;
class ProductionManager {
private:
    static ProductionManager *instance;
    map<CTokenType::Token, vector<Production*>*> productionMap;
    ProductionManager() {};
    ProductionManager(const ProductionManager& productionManager) {};
    void addProduction(Production &production);
public:
    static ProductionManager* getInstance();
    vector<Production*> *getProduction(CTokenType::Token left);
    void initProductions();

    struct ProductionComparator {
        Production &value;
        ProductionComparator(Production &production):value(production) {}
        bool operator()(Production * production) {
            return value == *production;
        }
    };
};


#endif //CPARSE_PRODUCTIONMANAGER_H
