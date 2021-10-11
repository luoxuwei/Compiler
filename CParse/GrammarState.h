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
    bool transitionDone = false;
    bool printInfo = true;
    vector<Production*> *productions;
    vector<Production*> closureSet;
    map<CTokenType::Token, vector<Production*>*> partition;
    map<CTokenType::Token, GrammarState*> transition;
    ProductionManager *productionManager = ProductionManager::getInstance();
    GrammarStateManager *grammarStateManager;
    vector<Production *> mergedProduction;//压缩节点时，将合并过来的节点的表达式添加到这个集合
    void makeClosure();
    void doPartition();
    void makeTransition();
    GrammarState * makeNextGrammarState(CTokenType::Token token);
    void extendFollowingTransition();
    void removeRedundantProduction(Production *production);
public:
    int stateNum;
    GrammarState(vector<Production*> *productions);
    static void increateStateNum();
    bool operator==(const GrammarState &grammarState);
    void createTransition();
    void print();
    void printClosure();
    void printPartition();
    bool checkProductionEqual(const GrammarState &grammarState, bool isPartial);
    void stateMerge(GrammarState *state);
};


#endif //CPARSE_GRAMMARSTATE_H
