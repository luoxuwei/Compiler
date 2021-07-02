#include <iostream>
#include "FiniteStateMachine.h"
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("need content\n");
        return 0;
    }
    char* content = argv[1];
    FiniteStateMachine finiteStateMachine;
    finiteStateMachine.lex(content, strlen(content));
    return 0;
}
