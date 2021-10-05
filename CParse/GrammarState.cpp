//
// Created by 罗旭维 on 2021/10/5.
//

#include "GrammarState.h"

int GrammarState::stateNumCount = 0;

GrammarState::GrammarState(vector<Production *> *productions): productions(productions) {
    stateNum = stateNumCount;
}

void GrammarState::increateStateNum() {
    stateNumCount++;
}