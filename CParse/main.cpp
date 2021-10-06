#include <iostream>
#include "ProductionManager.h"
#include "GrammarStateManager.h"
int main() {
    ProductionManager::getInstance()->initProductions();
    ProductionManager::getInstance()->printAllProductions();

    GrammarStateManager::getInstance()->buildTransitionStateMachine();
    return 0;
}
