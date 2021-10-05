//
// Created by 罗旭维 on 2021/10/5.
//

#include "GrammarState.h"
#include <stack>

int GrammarState::stateNumCount = 0;

GrammarState::GrammarState(vector<Production *> *productions): productions(productions) {
    stateNum = stateNumCount;
    closureSet.insert(closureSet.end(), productions->begin(), productions->end());
}

void GrammarState::increateStateNum() {
    stateNumCount++;
}

void GrammarState::makeClosure() {
    stack<Production *> productionStack;
    for_each(productions->begin(), productions->end(), [&productionStack](Production *production) -> void {
        productionStack.push(production);
    });
    Production *production = NULL;
    while (!productionStack.empty()) {
        production = productionStack.top();
        productionStack.pop();
        CTokenType::Token symbol = production->getDotSymbol();
        vector<Production *> *closures = productionManager->getProduction(symbol);
        for_each(closures->begin(), closures->end(), [&](Production *production1) -> void {
            if (find_if(closureSet.begin(), closureSet.end(), [&](Production *production2) -> bool {
                return *production1 == *production2;
            }) == closureSet.end()) {
                closureSet.push_back(production1);
                productionStack.push(production1);
            }
        });
    }
}

void GrammarState::doPartition() {
    for (auto production : closureSet) {
        CTokenType::Token token = production->getDotSymbol();
        if (token == CTokenType::Token::UNKNOWN_TOKEN) {
            continue;
        }
        auto iter = partition.find(token);
        vector<Production*> *productionList = NULL;
        if (iter == partition.end()) {
            productionList = new vector<Production*>();
            partition[token] = productionList;
        } else {
            productionList = iter->second;
        }
        if (find_if(productionList->begin(), productionList->end(), [&production](Production *p) -> bool {
            return *production == *p;
        }) == productionList->end()) {
            productionList->push_back(production);
        }
    }
}