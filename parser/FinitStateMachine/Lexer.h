//
// Created by 罗旭维 on 2021/7/2.
//

#ifndef FINITSTATEMACHINE_LEXER_H
#define FINITSTATEMACHINE_LEXER_H
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>

using std::string;

class Lexer {
private:
    char *buf = NULL;
    size_t buf_len = 0;


public:
    Lexer(char *content, size_t len) {
        if (content == NULL || len <= 0) {
            return;
        }
        buf = new char[len];
        memcpy(buf, content, len);
    }

    ~Lexer() {
        if (buf != NULL) {
            delete buf;
        }
    }

    char * text() {

    }

    int length() {

    }

    char * pre_text() {

    }

    char advance() {

    }

    int mark_start() {

    }

    int mark_end() {

    }

    int to_mark() {

    }

    int mark_prev() {

    }

    char lookahead(int n) {

    }

};


#endif //FINITSTATEMACHINE_LEXER_H
