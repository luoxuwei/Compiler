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
    /*
   * 要生成新的状态节点时，需要查找给定表达式所对应的节点是否已经存在，如果存在，就不必要构造
   * 新的节点
   */
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
        /*
        * 压缩时，把相似的节点找到，然后将相似节点合并
        */
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
    GrammarState *grammarState = getGrammarState(ProductionManager::getInstance()->getProduction(CTokenType::Token::PROGRAM));

    /*
    * 初始化节点0后，开始构建整个状态机网络，网络的构建类似一种链式反应，节点0生成节点1到5，每个子节点继续生成相应节点
    */
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

GrammarState * GrammarStateManager::getGrammarState(int stateNum) {
    auto begin = stateList.begin();
    auto end = stateList.end();
    if (isTransitionTableCompressed) {
        begin = compressedStateList.begin();
        end = compressedStateList.end();
    }
    auto ret = find_if(begin, end, [=](GrammarState *state) ->bool {
        return state->stateNum == stateNum;
    });
    if (ret != end) return *ret;
    return NULL;
}

