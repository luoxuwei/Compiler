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
#define EOF 0
class Lexer {
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
    Lexer(char *content, size_t len) {
        if (content == NULL || len <= 0) {
            return;
        }
        buf = new char[len];
        buf_len = len;
        memcpy(buf, content, len);
    }

    ~Lexer() {
        if (buf != NULL) {
            delete buf;
            buf = NULL;
        }
    }

    char * text() {
        int len = length();
        char *res = new char[len+1];
        memcpy(res, buf + cur_start, len);
        res[len] = 0;
        return res;
    }

    int length() {
        return cur_end - cur_start;
    }

    int line() {
        return lineno;
    }

    char * pre_text() {
        int len = pre_length();
        char *res = new char[len+1];
        memcpy(res, buf + pre_start, len);
        res[len] = 0;
        return res;
    }

    int pre_length() {
        return cur_start - pre_start;
    }

    char advance() {
        if (next >= buf_len) {
            return EOF;
        }
        if (buf[next] == '\n') {
            lineno++;
        }
        return buf[next++];
    }

    int mark_start() {
        cur_end = cur_start = next;
        return cur_start;
    }

    int mark_end() {
        cur_end = next;
        return cur_end;
    }

    int to_mark() {
        next = cur_end;
        return next;
    }

    int mark_prev() {
        pre_start = cur_start;
        pre_lineno = lineno;
        return pre_start;
    }

    char lookahead(int n) {
        if (next + n -1 >= buf_len) {
            return EOF;
        }

        return buf[next + n -1];
    }
};


#endif //FINITSTATEMACHINE_LEXER_H
