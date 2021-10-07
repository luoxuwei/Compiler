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

CLexer::~CLexer() {
    if (buf != NULL) {
        delete buf;
        buf = NULL;
    }
}

void CLexer::advance() {
    lookAhead = lex();
    while (lookAhead == CTokenType::Token::WHITE_SPACE) {
        lookAhead = lex();
    }
}

CTokenType::Token CLexer::isKeyWord(string &str) {
    if (keywordMap.find(str) != keywordMap.end()) {
        return keywordMap[str];
    }
    return CTokenType::Token::NAME;
}

CTokenType::Token CLexer::id_keyword_or_number() {
    if (isalpha(buf[charIndex])) {
        string text(buf + charIndex, textLen);
        return isKeyWord(text);
    } else if (isNum()) {
        return CTokenType::Token::NUMBER;
    }
    return CTokenType::Token::UNKNOWN_TOKEN;
}

CTokenType::Token CLexer::lex() {
    if (buf == NULL) {
        return CTokenType::Token::UNKNOWN_TOKEN;
    }

    charIndex = charIndex + textLen;
    if (charIndex >= buf_len) return CTokenType::Token::SEMI;

    textLen = 0;
    switch (buf[charIndex]) {
        case ';': textLen = 1; return CTokenType::Token::SEMI;
        case '+': textLen = 1; return CTokenType::Token::PLUS;
        case '*': textLen = 1; return CTokenType::Token::STAR;
        case '(': textLen = 1; return CTokenType::Token::LP;
        case ')': textLen = 1; return CTokenType::Token::RP;
        case ',': textLen = 1; return CTokenType::Token::COMMA;

        case '\n':
        case '\t':
        case ' ':
            textLen = 1;
            return CTokenType::Token::WHITE_SPACE;
        default:
            if (isalnum(buf[charIndex]) == false) {
                return CTokenType::Token::UNKNOWN_TOKEN;
            }
            else {
                int i = charIndex;
                while (i < buf_len && isalnum(buf[i])) {
                    i++;
                    textLen++;
                }

                return id_keyword_or_number();
            }

    }


}

bool CLexer::isNum() {
    int i = 0;
    if (buf[charIndex] == '-' || buf[charIndex] == '+') {
        i++;
    }
    for (; i<textLen; i++) {
        if (!isdigit(buf[charIndex+i])) return false;
    }
    return true;
}