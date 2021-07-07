//
// Created by 罗旭维 on 2021/7/7.
//

#ifndef REGEX_DFA_H
#define REGEX_DFA_H
#include "NFA.h"
#include <set>
#include <map>
#include <vector>
#define ASCII_COUNT 127
//假定DFA状态机节点数不会超过254个
#define MAX_DFA_STATE_COUNT 254
class DFA {
public:
    const static int STATE_FAILURE = -1;
    typedef uint32_t state_t;



    struct State {
        std::set<NFA::State*> nfaStates; //dfa节点对应的nfa节点集合
        state_t id; //节点编号
        bool accepted = false; //是不是可接受状态
        bool hasNfaSet(std::set<NFA::State*> &set) {
            return nfaStates == set;
        }
    };
    DFA();
    State * getDfaFromNfaSet(std::set<NFA::State*> &nfaSet);
    void construct(NFA &nfa);
    void printDfa();
private:
    std::deque<State> states;
    int dfaStateTransFormTable[MAX_DFA_STATE_COUNT][ASCII_COUNT];
    std::vector<State *> dfaList;

    void printDfaState(State *state);
    State *isNfaStatesExistInDfa(std::set<NFA::State*> &set);
    bool isOnNumberClass(int from, int to);
    bool isOnDot(int from, int to);
};


#endif //REGEX_DFA_H
