//
// Created by 罗旭维 on 2021/7/7.
//

#include "DFA.h"

DFA::DFA() {

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
    }
    return ret;
}

void DFA::construct(NFA &nfa) {
    std::set<NFA::State*> next, cur;
    cur.insert(nfa.startState());
    nfa.e_closure(cur);
}