//
// Created by 罗旭维 on 2021/10/5.
//

#ifndef CPARSE_GRAMMARSTATEMANAGER_H
#define CPARSE_GRAMMARSTATEMANAGER_H
#include <vector>
#include "GrammarState.h"
using namespace std;
class GrammarStateManager {
public:
    typedef map<int, map<CTokenType::Token, int>> LRStateTable;
private:
    vector<GrammarState *> stateList;
    vector<GrammarState*> compressedStateList;
    static GrammarStateManager *instance;
    bool isTransitionTableCompressed = true;
    map<GrammarState*, map<CTokenType::Token, GrammarState*>> transition;
    LRStateTable lrStateTable;
    GrammarStateManager(){}
    GrammarStateManager(const GrammarStateManager& grammarStateManager){}
    GrammarState *getAndMergeSimilarState(GrammarState *state);
    void printStateMap();
public:
    static GrammarStateManager *getInstance();
    GrammarState* getGrammarState(vector<Production *> *pl);
    void buildTransitionStateMachine();
    struct GrammarStateComparetor {
        GrammarState *value;
        GrammarStateComparetor(GrammarState *grammarState) :value(grammarState){}
        bool operator()(GrammarState *grammarState) {
            return *value == *grammarState;
        }
    };
    void addTransition(GrammarState *from, GrammarState *to, CTokenType::Token on);
    LRStateTable * getLRStateTable();
    void printReduceInfo();
    GrammarState* getGrammarState(int);
};


#endif //CPARSE_GRAMMARSTATEMANAGER_H
