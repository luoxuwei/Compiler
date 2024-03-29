#include <iostream>
#include "ProductionManager.h"
#include "GrammarStateManager.h"
#include "LRStateTableParser.h"
#include "CLexer.h"
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("need a paramter: filename\n");
        return 0;
    }
    char* path = argv[1];
    ProductionManager::getInstance()->initProductions();
    ProductionManager::getInstance()->printAllProductions();

    GrammarStateManager::getInstance()->buildTransitionStateMachine();
    CLexer lexer(path);
    LRStateTableParser parser(&lexer);
    parser.parse();
    return 0;
}
