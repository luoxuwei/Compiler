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

GrammarStateManager::LRStateTable * GrammarStateManager::getLRStateTable() {
    auto begin = stateList.begin();
    auto end = stateList.end();
    if (isTransitionTableCompressed) {
        begin = compressedStateList.begin();
        end = compressedStateList.end();
    }

    while (begin != end) {
        auto s = *begin;
        begin++;
        map<CTokenType::Token, int> jump;
        if (transition.find(s) != transition.end()) {
            for (auto i : transition[s]) {
                jump[i.first] = i.second->stateNum;
            }
        }

        map<CTokenType::Token, int > reduceMap = s->makeReduce();
        if (reduceMap.size() > 0) {
            for (auto ri : reduceMap) {
                jump[ri.first] = -ri.second;
            }
        }
        lrStateTable[s->stateNum] = jump;
    }
    return &lrStateTable;
}

GrammarState * GrammarStateManager::getGrammarState(vector<Production *> *pl) {
    GrammarState *state = new GrammarState(pl);
    if (std::find_if(stateList.begin(), stateList.end(), GrammarStateComparetor(state)) == stateList.end()) {
        stateList.push_back(state);
        GrammarState::increateStateNum();
        return state;
    }

    auto iter = std::find_if(stateList.begin(), stateList.end(), GrammarStateComparetor(state));
    delete state;
    delete pl;
    if (iter != stateList.end()) {
        return *iter;
    }
    return NULL;
}

void GrammarStateManager::addTransition(GrammarState *from, GrammarState *to, CTokenType::Token on) {
    if (isTransitionTableCompressed) {
        from = getAndMergeSimilarState(from);
        to = getAndMergeSimilarState(to);
    }

    auto iter = transition.find(from);
    if (iter == transition.end()) {
        transition[from] = map<CTokenType::Token, GrammarState *>();
    }
    printf("\nadd transition from: \n");
    from->print();
    printf("\non: %s to : \n", CTokenType::getSymbolStr(on));
    to->print();
    transition[from][on] = to;
}

GrammarState * GrammarStateManager::getAndMergeSimilarState(GrammarState *state) {
    auto iter = stateList.begin();
    GrammarState *currentState = NULL, *returnState = state;
    while (iter != stateList.end()) {
        currentState = *iter;
        iter++;
        if (!(*currentState == *state) && currentState->checkProductionEqual(*state, true)) {
            printf("\n Find similar state: \n");
            currentState->print();
            printf("\n==============\n");
            state->print();

            //一般是将编号大的合并到编号小的节点里去
            if (currentState->stateNum < state->stateNum) {
                currentState->stateMerge(state);
                returnState = currentState;
            } else {
                state->stateMerge(currentState);
                returnState = state;
            }

            printf("\ncombind state is: \n");
            returnState->print();
            break;
        }
    }

    if (find_if(compressedStateList.begin(), compressedStateList.end(), GrammarStateComparetor(returnState)) == compressedStateList.end()) {
        compressedStateList.push_back(returnState);
    }

    return returnState;
}

void GrammarStateManager::buildTransitionStateMachine() {
    GrammarState *grammarState = getGrammarState(ProductionManager::getInstance()->getProduction(CTokenType::Token::stmt));
    grammarState->createTransition();
    printf("\nbuildTransitionStateMachine finish\n");
    printStateMap();
    printReduceInfo();
}

void GrammarStateManager::printStateMap() {
    printf("\nMap size is: %d\n", transition.size());

    for (auto entry : transition) {
        GrammarState *from = entry.first;
        printf("\n********begin to print a map row********\n");
        printf("from state: \n");
        from->print();

        for (auto item : entry.second) {
            CTokenType::Token symbol = item.first;
            printf("on symbol: %s\n" , CTokenType::getSymbolStr(symbol));
            printf("to state: \n");
            GrammarState *to = item.second;
            to->print();
        }

        printf("\n********end a map row********\n");
    }
}

void GrammarStateManager::printReduceInfo() {
    printf("\nShow reduce for each state: \n");
    auto begin = stateList.begin();
    auto end = stateList.end();
    if (isTransitionTableCompressed) {
        begin = compressedStateList.begin();
        end = compressedStateList.end();
    }

    while (begin != end) {
        auto s = *begin;
        begin++;
        s->print();
        map<CTokenType::Token, int > reduceMap = s->makeReduce();
        if (reduceMap.size() == 0) {
            printf("in this state, can not take any reduce action\n");
        }
        for (auto i : reduceMap) {
            printf("Reduce on symbol: %s to Production %d\n", CTokenType::getSymbolStr(i.first), i.second);
        }
    }
}

