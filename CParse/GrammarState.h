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
public:
    GrammarState(vector<Production*> *productions);
    static void increateStateNum();
    bool operator==(GrammarState &grammarState);
    void createTransition();
};


#endif //CPARSE_GRAMMARSTATE_H
