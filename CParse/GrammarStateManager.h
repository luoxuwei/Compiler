//
// Created by 罗旭维 on 2021/10/5.
//

#ifndef CPARSE_GRAMMARSTATEMANAGER_H
#define CPARSE_GRAMMARSTATEMANAGER_H
#include <vector>
#include "GrammarState.h"
using namespace std;
class GrammarStateManager {
private:
    vector<GrammarState *> stateList;
    static GrammarStateManager *instance;
    bool isTransitionTableCompressed = true;
    map<GrammarState*, map<CTokenType::Token, GrammarState*>> transition;
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
};


#endif //CPARSE_GRAMMARSTATEMANAGER_H
