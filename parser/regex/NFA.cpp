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

void NFA::printNfa() {
    printf("\n");
    printNfa(start);
}

void NFA::printNfa(State *state) {
    if (state == NULL || state->visited) return;

    if (state == start) {
        printf("--------NFA-------- \n");
    }

    state->visited = true;
    printNfaNode(state);

    if (state == start) {
        printf("  (START STATE)");
    }

    printf("\n");
    printNfa(state->next);
    printNfa(state->next2);
}

void NFA::printNfaNode(State *state) {
    if (state->next == NULL) {
        printf("TERMINAL");
    } else {
        printf("NFA state: %d ---> %d", state->id, state->next->id);
        if (state->next2 != NULL) {
            printf(" %d", state->next2->id);
        }
        printf(" on: ");
        switch (state->edge) {
            case CCL:
                printCCL(state);
                break;
            case EPSILON:
                printf("EPSILON ");
                break;
            default:
                printf("%c", (char)state->edge);
        }
    }

}

void NFA::printCCL(State *state) {
    printf("[ ");
    for (int i = 0; i < ASCII_COUNT; i++) {
        if (state->inputSet.test(i)) {
            if (i < ' ') {
                printf("^%c", (char)(i + '@'));
            }
            else {
                printf("%c", (char)i);
            }
        }
    }

    printf(" ]");
}