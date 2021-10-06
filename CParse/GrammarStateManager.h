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
    GrammarStateManager(){}
    GrammarStateManager(const GrammarStateManager& grammarStateManager){}

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
};


#endif //CPARSE_GRAMMARSTATEMANAGER_H
