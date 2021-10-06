//
// Created by 罗旭维 on 2021/10/5.
//

#include "GrammarState.h"
#include "GrammarStateManager.h"
#include <stack>

int GrammarState::stateNumCount = 0;

GrammarState::GrammarState(vector<Production *> *productions): productions(productions) {
    stateNum = stateNumCount;
    closureSet.insert(closureSet.end(), productions->begin(), productions->end());
    grammarStateManager = GrammarStateManager::getInstance();
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
        if (closures == NULL) continue;
        for_each(closures->begin(), closures->end(), [&](Production *production1) -> void {
            if (find_if(closureSet.begin(), closureSet.end(), [&](Production *production2) -> bool {
                return *production1 == *production2;
            }) == closureSet.end()) {
                closureSet.push_back(production1);
                productionStack.push(production1);
            }
        });
    }

    printClosure();
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

    printPartition();
}

void GrammarState::makeTransition() {
    for (auto pair : partition) {
        printf("\n====begin print transition info ====");
        GrammarState *nextState = makeNextGrammarState(pair.first);
        transition[pair.first] = nextState;
        printf("from state %d to state %d on %s \n", stateNum, nextState->stateNum, CTokenType::getSymbolStr(pair.first));
        printf("----state %d ---- \n", nextState->stateNum);
        nextState->print();
    }
    extendFollowingTransition();
}

GrammarState * GrammarState::makeNextGrammarState(CTokenType::Token token) {
    auto iter = partition.find(token);
    if (iter == partition.end()) return NULL;
    vector<Production*> *newStateProductionList = new vector<Production*>();
    for (auto p : *(iter->second)) {
        newStateProductionList->push_back(p->doForword());
    }
    return grammarStateManager->getGrammarState(newStateProductionList);
}

void GrammarState::extendFollowingTransition() {
    for (auto pair : transition) {
        if (!pair.second->transitionDone) {
            pair.second->createTransition();
        }
    }
}

bool GrammarState::operator==(GrammarState &grammarState) {
    if (productions->size() != grammarState.productions->size()) return false;
    return std::equal(productions->begin(), productions->end(), grammarState.productions->begin(), [](Production* p1, Production *p2) -> bool  {
        return *p1 == *p2;
    });
}

void GrammarState::createTransition() {
    if (transitionDone) return;
    transitionDone = true;

    printf("\n====make transition=====\n");
    print();

    makeClosure();
    doPartition();
    makeTransition();

    printInfo = false;
}

void GrammarState::print() {
    printf("state number: %d \n", stateNum);
    for (auto p : *productions) {
        p->print();
    }
}

void GrammarState::printClosure() {
    if (!printInfo) return;

    printf("closure set is: \n");
    for (auto p : closureSet) {
        p->print();
    }
}

void GrammarState::printPartition() {
    if (!printInfo) return;

    for (auto iter : partition) {
        printf("partition for symbol: %d \n", CTokenType::getSymbolStr(iter.first));
        for (auto p : (*iter.second)) {
            p->print();
        }
    }
}