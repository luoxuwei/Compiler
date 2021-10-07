//
// Created by 罗旭维 on 2021/10/5.
//

#include "GrammarStateManager.h"

GrammarStateManager *GrammarStateManager::instance = NULL;
GrammarStateManager * GrammarStateManager::getInstance() {
    if (instance == NULL) {
        instance = new GrammarStateManager();
    }
    return instance;
}

GrammarState * GrammarStateManager::getGrammarState(vector<Production *> *pl) {
    GrammarState *state = new GrammarState(pl);
    if (std::find_if(stateList.begin(), stateList.end(), GrammarStateComparetor(state)) == stateList.end()) {
        stateList.push_back(state);
        GrammarState::increateStateNum();
        return state;
    }

    auto iter = std::find_if(stateList.begin(), stateList.end(), GrammarStateComparetor(state));
    if (iter != stateList.end()) {
        return *iter;
    }
    return NULL;
}

void GrammarStateManager::buildTransitionStateMachine() {
    GrammarState *grammarState = getGrammarState(ProductionManager::getInstance()->getProduction(CTokenType::Token::stmt));
    grammarState->createTransition();
    printf("\nbuildTransitionStateMachine finish\n");
}