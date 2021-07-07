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

void NFA::match(std::string &str) {
    LexerBuffer lexerBuffer(str.data(), str.length());
    std::set<NFA::State*> next, cur;
    cur.insert(startState());
    e_closure(cur);
    char c;
    bool lastAccepted = false;
    while ((c = (char) lexerBuffer.advance()) != EOF) {
        move(cur, next, c);
        e_closure(next);
        if (!next.empty()) {
            if (hasAcceptState(next)) {
                lastAccepted = true;
            }
        } else {
            break;
        }
        cur = next;
        next.clear();
    }
    if (lastAccepted) {
        printf("The Nfa Machine can recognize string: %s \n", str.c_str());
    }
}

bool NFA::hasAcceptState(std::set<NFA::State *> &set) {
    std::string str("Accept State: ");
    bool isAccepted = false;
    std::set<NFA::State*>::iterator iterator = set.begin();
    State *state;
    while (iterator != set.end()) {
        state = *iterator;
        iterator++;
        if (state->next == NULL && state->next2 == NULL) {
            isAccepted = true;
            str.append(std::to_string(state->id));
            str.append(" ");
        }
    }
    if (isAccepted) {
        printf("%s \n", str.c_str());
    }
    return isAccepted;
}

/*
* 计算in集合中nfa节点所对应的ε闭包，
* 并将闭包的节点加入到in中
*/
void NFA::e_closure(std::set<NFA::State *> &in) {
    if (in.empty()) return;
    if (debug) {
        printf("ε-Closure( %s ) = ", stringFromNfa(in).c_str());
    }

    std::stack<NFA::State*> stateStack;
    std::set<NFA::State *>::iterator iterator = in.begin();
    while (iterator != in.end()) {
        stateStack.push(*iterator);
        iterator++;
    }

    NFA::State *cur;
    while (!stateStack.empty()) {
        cur = stateStack.top();
        stateStack.pop();
        if (cur->edge == NFA::EPSILON && cur->next != NULL) {
            stateStack.push(cur->next);
            in.insert(cur->next);
        }
        if (cur->next2 != NULL) {
            stateStack.push(cur->next2);
            in.insert(cur->next2);
        }
    }

    if (debug) {
        printf("{ %s } \n", stringFromNfa(in).c_str());
    }
}

std::string NFA::stringFromNfa(std::set<NFA::State *>& set) {
    std::set<NFA::State *>::iterator iterator = set.begin();
    std::string ret;

    while (iterator != set.end()) {
        ret.append(std::to_string((*iterator)->id));
        ret.append(",");
        iterator++;
    }
    return ret;
}

void NFA::move(std::set<NFA::State *> &in, std::set<NFA::State *> &out, char c) {
    std::set<NFA::State *>::iterator iterator = in.begin();
    NFA::State *state;
    while (iterator != in.end()) {
        state = *iterator;
        iterator++;
        if (state->edge == c || (state->edge == NFA::CCL && state->inputSet.test(c))) {
            out.insert(state->next);
        }
    }

    if (debug) {
        printf("move({ %s }, ' %c ')= ", stringFromNfa(in).c_str(), c);
        printf("{ %s } \n", stringFromNfa(out).c_str());
    }
}