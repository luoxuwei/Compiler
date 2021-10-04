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
    int state = 0;                   //开始时处于状态0
    int lastaccept =  STATE_FAILURE; //上一个状态
    int nstate =  STATE_FAILURE;     //通过look ahead 字符得到的下一个状态
    char look = EOF;                 //预读取字符
public:
    FiniteStateMachine() {

    }

    void lex(char *content, int len) {
        lexer = new Lexer(content, len);
        lexer->mark_start();
        while (true) {
            if ((look = lexer->lookahead(1)) != EOF) {
                nstate = fms.next(state, look);
            } else {
                nstate = STATE_FAILURE;
            }

            if (nstate != STATE_FAILURE) {
                printf("Transition from state : %d to state :  %d on input char: %c \n", state, nstate, look);
                lexer->advance();
                if (fms.isAcceptSate(nstate)) {
                    lastaccept = nstate;
                    lexer->mark_end();
                }
                state = nstate;
            } else {
                if (lastaccept == STATE_FAILURE) {
                    if (look != '\n') {
                        printf("Ingoring bad input \n");
                    }
                    lexer->advance();
                } else {
                    printf("Acceting state: %d \n", lastaccept);
                    printf("line: %d  accept text: %s", lexer->line(), lexer->text());
                    switch (lastaccept) {
                        case 1:
                            printf(" it is a Integer \n");
                            break;
                        case 2:
                        case 4:
                            printf(" it is a float point number \n");
                            break;
                        default:
                            assert(false);
                    }
                }
                //识别到错误字符或给出判断后，将状态机重置
                lastaccept = STATE_FAILURE;
                state = 0;
                lexer->mark_start();
            }

            if (look == EOF) {
                break;
            }

        }

        delete lexer;
    }


};


#endif //FINITSTATEMACHINE_FINITESTATEMACHINE_H
