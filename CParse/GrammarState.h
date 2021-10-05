//
// Created by 罗旭维 on 2021/10/5.
//

#ifndef CPARSE_GRAMMARSTATE_H
#define CPARSE_GRAMMARSTATE_H
#include <vector>
#include <map>
#include "Production.h"
#include "ProductionManager.h"
#include "CTokenType.h"

using namespace std;
class GrammarState {
private:
    static int stateNumCount;
    int stateNum;
    vector<Production*> *productions;
    vector<Production*> closureSet;
    map<CTokenType::Token, vector<Production*>*> partition;
    ProductionManager *productionManager = ProductionManager::getInstance();
    void makeClosure();
    void doPartition();
public:
    GrammarState(vector<Production*> *productions);
    static void increateStateNum();
};


#endif //CPARSE_GRAMMARSTATE_H
