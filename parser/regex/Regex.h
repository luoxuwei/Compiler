//
// Created by 罗旭维 on 2021/7/3.
//

#ifndef REGEX_REGEX_H
#define REGEX_REGEX_H
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <algorithm>
#include "ExprLexer.h"
#include "NFA.h"


class Regex {
private:
    ExprLexer* exprLexer;
    NFA nfa;


public:
    Regex(const char * macroFilePath, char * regex);

    ~Regex() {
        if (exprLexer != NULL) {
            delete exprLexer;
        }
    }

    void parse();
    /*
     *term ->  character | [...] | [^...]
     * */
    bool term();
    bool character();
    bool dot();
};


#endif //REGEX_REGEX_H
