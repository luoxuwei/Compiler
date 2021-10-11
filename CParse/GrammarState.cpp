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

void GrammarState::stateMerge(GrammarState *state) {
    for (auto p : *state->productions) {
        if (find(productions->begin(), productions->end(), p) == productions->end()
                && find(mergedProduction.begin(), mergedProduction.end(), p) == mergedProduction.end()) {
            mergedProduction.push_back(p);
        }
    }
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
    printf("------begin make closure-------\n");
    while (!productionStack.empty()) {
        production = productionStack.top();
        productionStack.pop();
        printf("production on top of stack is : \n");
        production->print();
        CTokenType::Token symbol = production->getDotSymbol();
        if (CTokenType::isTerminal(symbol)) {
            printf("symbol after dot is not non-terminal, ignore and press next item\n");
            continue;
        }

        vector<Production *> *closures = productionManager->getProduction(symbol);
        if (closures == NULL) continue;
        vector<CTokenType::Token> lookAhead = production->computeFirstSetOfBetaAndC();
        for_each(closures->begin(), closures->end(), [&](Production *production1) -> void {
            Production *newProduct = production1->cloneSelf();
            newProduct->addLookAheadSet(lookAhead);
            printf("\ncreate new production \n");
            newProduct->print();
            if (find_if(closureSet.begin(), closureSet.end(), [&](Production *production2) -> bool {
                return *newProduct == *production2;
            }) == closureSet.end()) {
                printf("push and add new production to stack and closureSet\n");
                closureSet.push_back(newProduct);
                productionStack.push(newProduct);
                removeRedundantProduction(newProduct);
            } else {
                printf("\nthe production is already exist!\n");
                delete newProduct;
            }
        });
    }

    printClosure();
    printf("\n--------------end make closure-----------------\n");
}

void GrammarState::removeRedundantProduction(Production *production) {
    bool removeHappended = true;
    while(removeHappended) {
        removeHappended = false;
        auto iter = closureSet.begin();
        while (iter != closureSet.end()) {
            Production *item = *iter;
            if (production->coverUp(*item)) {
                removeHappended = true;
                closureSet.erase(iter);
                printf("remove redundant production: ");
                item->print();
                break;
            }
            iter++;
        }
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

    printPartition();
}

void GrammarState::makeTransition() {
    for (auto pair : partition) {
        printf("\n====begin print transition info ====\n");
        GrammarState *nextState = makeNextGrammarState(pair.first);
        transition[pair.first] = nextState;
        printf("from state %d to state %d on %s \n", stateNum, nextState->stateNum, CTokenType::getSymbolStr(pair.first));
        printf("----state %d ---- \n", nextState->stateNum);
        nextState->print();
        GrammarStateManager::getInstance()->addTransition(this, nextState, pair.first);
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

bool GrammarState::operator==(const GrammarState &grammarState) {
    return checkProductionEqual(grammarState, false);
}

bool GrammarState::checkProductionEqual(const GrammarState &grammarState, bool isPartial) {
    if (productions->size() != grammarState.productions->size()) return false;

    int equalCount = 0;
    for (int i=0; i<grammarState.productions->size(); i++) {
        for (int j=0; j<productions->size(); j++) {
            if (!isPartial) {
                if (*(grammarState.productions->at(i)) == *(productions->at(j))) {
                    equalCount++;
                    break;
                }
            } else {
                if (grammarState.productions->at(i)->productionEquals(*productions->at(j))) {
                    equalCount++;
                    break;
                }
            }
        }
    }

    return equalCount == grammarState.productions->size();
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
    for (int i = 0; i < mergedProduction.size(); i++) {
        mergedProduction.at(i)->print();
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
        printf("partition for symbol: %s \n", CTokenType::getSymbolStr(iter.first));
        for (auto p : (*iter.second)) {
            p->print();
        }
    }
}