//
// Created by 罗旭维 on 2021/7/2.
//

#ifndef FINITSTATEMACHINE_LEXER_H
#define FINITSTATEMACHINE_LEXER_H
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include "util.h"

using std::string;
#define EOF 0
class LexerBuffer {
private:
    char *buf = NULL;
    size_t buf_len = 0;
    int next=0;              //指向当前要读入的字符位置
    int cur_start = 0;     //当前被词法分析器分析的字符串位置
    int cur_end = 0;       //当前被词法分析器分析的字符串结束位置
    int pre_start = 0;     //上一个被词法分析器分析的字符串起始位置
    int pre_lineno = 0;    //上一个被词法分析器分析的字符串所在的行号
    int pre_len = 0;       //上一个被词法分析器分析的字符串长度

    int lineno = 1;        //

public:
    LexerBuffer(const char *filePath);

    LexerBuffer(const char *content, size_t len);

    ~LexerBuffer();

    char * text();

    int length() {
        return cur_end - cur_start;
    }

    int line() {
        return lineno;
    }

    char * pre_text();

    int pre_length() {
        return cur_start - pre_start;
    }

    char advance();

    int mark_start();

    int mark_end();

    int to_mark();

    int mark_prev();

    char lookahead(int n);
};


#endif //FINITSTATEMACHINE_LEXER_H
