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
    int lookAhead = CTokenType::Token::UNKNOWN_TOKEN;
    char *buf = NULL;
    size_t buf_len = 0;
    map<string, int> keywordMap;
public:
    CLexer(const char *filePath);
    CLexer(const char *content, size_t len);
    CLexer();

};


#endif //CPARSE_CLEXER_H
