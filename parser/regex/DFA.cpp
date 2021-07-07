//
// Created by 罗旭维 on 2021/7/7.
//

#include "DFA.h"

DFA::DFA() {
   for (int i=0; i<MAX_DFA_STATE_COUNT; i++) {
       for (int j=0; j<=ASCII_COUNT; j++) {
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

void DFA::minimize() {
    /*
     * 生成两个分区，分别将非接收状态节点和接收状态节点放入两个分区
     */
    StateGroup *group1 = createGroup();
    StateGroup *group2 = createGroup();
    std::vector<State *>::iterator iterator = dfaList.begin();
    while (iterator != dfaList.end()) {
        if ((*iterator)->accepted) {
            group1->add(*iterator);
        } else {
            group2->add(*iterator);
        }
        iterator++;
    }
    group1->print();
    group2->print();
    /*
     * 根据输入对每个分区进行分割，一但有新的分区生成，那么就必须对所有分区再进行分割
     * 因为新分区产生后，原来不可分割的分区可能就可以分割了
     */
    do {
        addNewGroup = false;
        doGroupSeperation();
    } while (addNewGroup);
}

/*
* 根据输入对每个分区进行分割，一但有新的分区生成，那么就必须对所有分区再进行分割
* 因为新分区产生后，原来不可分割的分区可能就可以分割了
*/
void DFA::doGroupSeperation() {
    DFA::State *first = NULL,  *next = NULL;
    StateGroup *group = NULL;

    //先对输入是数字字符时进行分割
    for (int i=0; i<groups.size(); i++) {
        newGroup = NULL;
        group = &groups.at(i);
        int dfaCount = 1;
        printf("Handle seperation on group: \n");
        group->print();
        first = group->get(0);
        next = group->get(dfaCount);
        while (next != NULL) {
            for (char c='0'; c<='9'; c++) {
                if (doGroupSeperationOnInput(group, first, next, c)) {
                    addNewGroup = true;
                    break;
                }
            }
            dfaCount++;
            next = group->get(dfaCount);
        }
        group->commitRemove();
    }

    //对输入是非数字字符时进行分割
    for (int i=0; i<groups.size(); i++) {
        newGroup = NULL;
        group = &groups.at(i);
        int dfaCount = 1;
        printf("Handle seperation on group: \n");
        group->print();
        first = group->get(0);
        next = group->get(dfaCount);
        while (next != NULL) {
            for (char c = 0; c <= ASCII_COUNT; c++) {
                if (!std::isdigit(c) && doGroupSeperationOnInput(group, first, next, c)) {
                    addNewGroup = true;
                    break;
                }
            }
            dfaCount++;
            next = group->get(dfaCount);
        }
        group->commitRemove();
    }

}

/*
 * 如果两个DFA节点跳转后的节点不在同一个分区，那么第二个节点必须从当前分区分割出去
 */
bool DFA::doGroupSeperationOnInput(StateGroup *group, DFA::State *first, DFA::State *next, char c) {
    state_t firstTo = dfaStateTransFormTable[first->id][c];
    state_t nextTo = dfaStateTransFormTable[next->id][c];
    if (getContainingGroup(firstTo) != getContainingGroup(nextTo)) {
        if (newGroup == NULL) {
            newGroup = createGroup();
        }
        newGroup->add(next);
        group->addToRemove(next);
        printf("Dfa:%d and Dfa:%d jump to different group on input char %c", first->id, next->id, c);
        printf("remove Dfa:%d from group:%d and add it to group:%d", next->id, group->id, newGroup->id);
        return true;
    }

    return false;
}

DFA::StateGroup * DFA::getContainingGroup(state_t state) {
    std::deque<StateGroup>::iterator iterator = groups.begin();
    while (iterator != groups.end()) {
        std::vector<DFA::State *>::iterator dfaItr = (*iterator).dfaGroup.begin();
        while (dfaItr != (*iterator).dfaGroup.end()) {
            if ((*dfaItr)->id == state) return &(*iterator);
            dfaItr++;
        }
        iterator++;
    }
    return NULL;
}