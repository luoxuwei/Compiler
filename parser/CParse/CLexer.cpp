//
// Created by 罗旭维 on 2021/9/28.
//
#include <fcntl.h>
#include <sys/stat.h>
#include "CLexer.h"
CLexer::CLexer() {
    keywordMap["auto"] = CTokenType::Token::CLASS;
    keywordMap["static"] = CTokenType::Token::CLASS;
    keywordMap["register"] = CTokenType::Token::CLASS;
    keywordMap["char"] = CTokenType::Token::TYPE;
    keywordMap["float"] = CTokenType::Token::TYPE;
    keywordMap["double"] = CTokenType::Token::TYPE;
    keywordMap["int"] = CTokenType::Token::TYPE;
    keywordMap["long"] = CTokenType::Token::TYPE;
    keywordMap["void"] = CTokenType::Token::TYPE;
}

CLexer::CLexer(const char *filePath): CLexer() {
    if (filePath != NULL) {
        FILE* f = fopen(filePath, "r");
        if (f == NULL) {
            assert(false);
        }
        struct stat statbuf;
        fstat(fileno(f), &statbuf);
        buf = new char[statbuf.st_size];
        buf_len = statbuf.st_size;
        fread(buf, statbuf.st_size, 1, f);
        fclose(f);
    } else {
        buf = NULL;
        buf_len = 0;
    }
}

CLexer::CLexer(const char *content, size_t len): CLexer() {
    if (content == NULL || len <= 0) {
        return;
    }
    buf = new char[len];
    buf_len = len;
    memcpy(buf, content, len);
}