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
class GrammarStateManager;

using namespace std;
class GrammarState {
private:
    static int stateNumCount;
    int stateNum;
    bool transitionDone = false;
    bool printInfo = true;
    vector<Production*> *productions;
    vector<Production*> closureSet;
    map<CTokenType::Token, vector<Production*>*> partition;
    map<CTokenType::Token, GrammarState*> transition;
    ProductionManager *productionManager = ProductionManager::getInstance();
    GrammarStateManager *grammarStateManager;
    void makeClosure();
    void doPartition();
    void makeTransition();
    GrammarState * makeNextGrammarState(CTokenType::Token token);
    void extendFollowingTransition();
    void removeRedundantProduction(Production *production);
public:
    GrammarState(vector<Production*> *productions);
    static void increateStateNum();
    bool operator==(GrammarState &grammarState);
    void createTransition();
    void print();
    void printClosure();
    void printPartition();
};


#endif //CPARSE_GRAMMARSTATE_H
