//
// Created by 罗旭维 on 2021/7/7.
//

#include "DFA.h"

DFA::DFA() {
   for (int i=0; i<MAX_DFA_STATE_COUNT; i++) {
       for (int j=0; j<ASCII_COUNT; j++) {
           dfaStateTransFormTable[i][j] = STATE_FAILURE;
       }
   }
}

DFA::State * DFA::getDfaFromNfaSet(std::set<NFA::State *> &nfaSet) {
    State *ret;
    states.resize(states.size() + 1);
    ret = &states.back();
    ret->id = states.size();
    std::set<NFA::State *>::iterator iterator = nfaSet.begin();
    while (iterator != nfaSet.end()) {
        ret->nfaStates.insert(*iterator);
        if ((*iterator)->next == NULL && (*iterator)->next2 == NULL) {
            ret->accepted = true;
        }
        iterator++;
    }
    return ret;
}

void DFA::construct(NFA &nfa) {
    nfa.debug = false;
    std::set<NFA::State*> next, cur;
    cur.insert(nfa.startState());
    nfa.e_closure(cur);
    DFA::State *state = getDfaFromNfaSet(cur);
    dfaList.push_back(state);

    printf("Create DFA start node: \n");
    printDfaState(state);

    int dfaIndex = 0;
    DFA::state_t nextState = DFA::STATE_FAILURE;

    while (dfaIndex < dfaList.size()) {
        state = dfaList.at(dfaIndex);
        for (int c=0; c < ASCII_COUNT; c++) {
            nfa.move(state->nfaStates, next, c);
            if (!next.empty()) {
                nfa.e_closure(next);
                DFA::State *dfa = isNfaStatesExistInDfa(next);
                if (dfa == NULL) {
                    printf("Create DFA node: \n");
                    DFA::State *nDfa = getDfaFromNfaSet(next);
                    printDfaState(nDfa);

                    nextState = nDfa->id;
                    dfaList.push_back(nDfa);
                } else {
                    printf("Get a existed dfa node: \n");
                    printDfaState(dfa);
                    nextState = dfa->id;
                }
            } else {
                nextState = STATE_FAILURE;
            }
            next.clear();
            if (nextState != STATE_FAILURE) {
                printf("DFA from state: %d to state:%d on char: %c\n", state->id, nextState, c);
            }
            dfaStateTransFormTable[state->id][c] = nextState;
        }

        dfaIndex++;
    }
    nfa.debug = true;
}

DFA::State *DFA::isNfaStatesExistInDfa(std::set<NFA::State *> &set) {
    std::vector<DFA::State *>::iterator iterator = dfaList.begin();
    while (iterator != dfaList.end()) {
        if ((*iterator)->hasNfaSet(set)) {
            return *iterator;
        }
        iterator++;
    }
    return NULL;
}

void DFA::printDfaState(DFA::State *state) {
    printf("Dfa state: %d its nfa states are: ", state->id);
    std::set<NFA::State*>::iterator iterator = state->nfaStates.begin();
    while (iterator != state->nfaStates.end()) {
        printf("%d,", (*iterator)->id);
        iterator++;
    }
    printf("\n");
}

void DFA::printDfa() {
    printf("\n");
    int dfaNum = dfaList.size();
    for (int i = 0; i < dfaNum; i++)
        for (int j = 0; j < dfaNum; j++) {
            if (isOnNumberClass(i,j)) {
                printf("From state %d to state %d on D \n", i, j);
            }

            if (isOnDot(i, j)) {
                printf("From state %d to state %d on dot \n", i, j);
            }
        }
}

bool DFA::isOnDot(int from, int to) {
    if (dfaStateTransFormTable[from]['.'] != to) {
        return false;
    }

    return true;
}

bool DFA::isOnNumberClass(int from, int to) {
    char c = '0';
    for (c = '0'; c <= '9'; c++) {
        if (dfaStateTransFormTable[from][c] != to) {
            return false;
        }
    }

    return true;
}