//
// Created by 罗旭维 on 2021/9/28.
//

#ifndef CPARSE_CLEXER_H
#define CPARSE_CLEXER_H

#include <cstdio>
#include <map>
#include <string>
#include "CTokenType.h"

using namespace std;

class CLexer {
private:
    CTokenType::Token lookAhead = CTokenType::Token::UNKNOWN_TOKEN;
    char *buf = NULL;
    size_t buf_len = 0;
    int charIndex = 0;
    int textLen = 0;
    map<string, CTokenType::Token> keywordMap;

    CTokenType::Token lex();
    CTokenType::Token isKeyWord(string& str);
    CTokenType::Token id_keyword_or_number();
    bool isNum();
    CTokenType::Token translateStringToToken();
public:
    CLexer(const char *filePath);
    CLexer(const char *content, size_t len);
    CLexer();
    ~CLexer();

    CTokenType::Token token() {
        return lookAhead;
    }

    void advance();
    string lookAheadText();
};


#endif //CPARSE_CLEXER_H
