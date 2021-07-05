//
// Created by 罗旭维 on 2021/7/5.
//

#include "NFA.h"
#include "util.h"

NFA::NFA() {

}

NFA::State * NFA::newNfa() {
    State *ret;
   if (++nfaStates > NFA_MAX) {
       parseErr(Error::E_MEM);
   }

   if (!stateStack.empty()) {
       ret = stateStack.top();
       stateStack.pop();
   } else {
       states.resize(states.size() + 1);
       ret = &states.back();
   }

    ret->clearState();
    ret->id = nfaStates;
    ret->edge = EPSILON;
    return ret;
}

void NFA::recycleNfa(State *state) {
    nfaStates--;
    state->clearState();
    stateStack.push(state);
}