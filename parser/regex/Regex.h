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
    void dodash(NFA::State *state);
    bool first_in_cat(ExprLexer::Token token);

public:
    Regex(const char * macroFilePath, char * regex);

    ~Regex() {
        if (exprLexer != NULL) {
            delete exprLexer;
        }
    }

    void parse();
    void printNfa();
    /*
     *term ->  character | [...] | [^...]
     * */
    bool term();
    bool character();
    bool dot();
    //字符集[..]和字符集取反[^...]
    bool charClass();
    //[]*, *闭包
    bool starClosure();
    //[]+，+闭包
    bool plusClosure();
    //?,0或1次，选择操作
    bool optionClosure();
    //factor -> term* | term+ | term?.
    void factor();
    //正则表达式连接操作 cat_expr -> factor factor .....
    void cat_expr();
};


#endif //REGEX_REGEX_H
