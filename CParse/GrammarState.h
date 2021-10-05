//
// Created by 罗旭维 on 2021/10/5.
//

#ifndef CPARSE_GRAMMARSTATE_H
#define CPARSE_GRAMMARSTATE_H
#include <vector>
#include "Production.h"
#include "ProductionManager.h"

using namespace std;
class GrammarState {
private:
    static int stateNumCount;
    int stateNum;
    vector<Production*> *productions;
    ProductionManager *productionManager = ProductionManager::getInstance();

public:
    GrammarState(vector<Production*> *productions);
    static void increateStateNum();
};


#endif //CPARSE_GRAMMARSTATE_H
