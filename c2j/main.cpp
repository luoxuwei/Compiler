#include <iostream>
#include "ProductionManager.h"
#include "GrammarStateManager.h"
#include "LRStateTableParser.h"
#include "CLexer.h"
#include "Intepretor.h"
#include "CodeTreeBuilder.h"
#include "ProgramGenerator.h"

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
    LRStateTableParser::getInstance()->parse(&lexer);

    ProgramGenerator::getInstance()->generateHeader();

    Intepretor::getInstance()->Execute(CodeTreeBuilder::getInstance()->getCodeTreeRoot());

    ProgramGenerator::getInstance()->finish();
    return 0;
}
