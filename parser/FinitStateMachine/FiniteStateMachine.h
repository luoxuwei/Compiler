//
// Created by 罗旭维 on 2021/7/2.
//

#ifndef FINITSTATEMACHINE_FINITESTATEMACHINE_H
#define FINITSTATEMACHINE_FINITESTATEMACHINE_H
#include "fms.h"
#include "Lexer.h"
class FiniteStateMachine {
private:
    Lexer *lexer = NULL;
    FMS fms;
public:
    FiniteStateMachine() {

    }

    void lex(char *content, int len) {
        lexer = new Lexer(content, len);
        lexer->mark_start();
        while (true) {

        }

        delete lexer;
    }


};


#endif //FINITSTATEMACHINE_FINITESTATEMACHINE_H
