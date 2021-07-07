//
// Created by 罗旭维 on 2021/7/7.
//

#ifndef REGEX_DFA_H
#define REGEX_DFA_H
#include "NFA.h"
#include <set>
#include <map>
#include <vector>
#define ASCII_COUNT 128
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
private:
    std::deque<State> states;
    std::map<state_t, std::map<int, state_t>> dfaStateTransFormTable;
    std::vector<State *> dfaList;
};


#endif //REGEX_DFA_H
