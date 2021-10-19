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
    keywordMap["enum"] = CTokenType::Token::ENUM;
    keywordMap["long"] = CTokenType::Token::TYPE;
    keywordMap["short"] = CTokenType::Token::TYPE;
    keywordMap["void"] = CTokenType::Token::TYPE;
    keywordMap["struct"] = CTokenType::Token::STRUCT;
    keywordMap["return"] = CTokenType::Token::RETURN;
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

    bool inString = false;
    textLen = 0;
    for (int i = charIndex; i < buf_len; i++) {
        switch (buf[charIndex]) {
            case ';': textLen = 1; return CTokenType::Token::SEMI;
            case '+': textLen = 1; return CTokenType::Token::PLUS;
            case '*': textLen = 1; return CTokenType::Token::STAR;
            case '(': textLen = 1; return CTokenType::Token::LP;
            case ')': textLen = 1; return CTokenType::Token::RP;
            case ',': textLen = 1; return CTokenType::Token::COMMA;
            case '{': textLen = 1; return CTokenType::Token::LC;
            case '}': textLen = 1; return CTokenType::Token::RC;
            case '=': textLen = 1; return CTokenType::Token::EQUAL;
            case '?': textLen = 1; return CTokenType::Token::QUEST;
            case ':': textLen = 1; return CTokenType::Token::COLON;
            case '&': textLen = 1; return CTokenType::Token::AND;
            case '|': textLen = 1; return CTokenType::Token::OR;
            case '/':
            case '%':
                textLen = 1;
                return CTokenType::Token::DIVOP;
            case '>':
            case '<':
                if (buf[i + 1] == '=') {
                    textLen = 2;
                } else if ((buf[i] == '<' && buf[i + 1] == '<') || (buf[i] == '>' && buf[i + 1] == '>')) {
                    textLen = 2;
                    return CTokenType::Token::SHIFTOP;
                } else {
                    textLen = 1;
                }
                return CTokenType::Token::RELOP;
            case '\n':
            case '\t':
            case ' ':
                textLen = 1;
                return CTokenType::Token::WHITE_SPACE;
            case '"':
                inString = !inString;
                if (!inString) {
                    return CTokenType::Token::STRING;
                }
                break;
            default:
                if (!isalnum(buf[charIndex])) {
                    return CTokenType::Token::UNKNOWN_TOKEN;
                }
                else {
                    while (i < buf_len && isalnum(buf[i]) || inString) {
                        i++;
                        textLen++;
                    }

                    if (i >= buf_len && inString) {
                        printf("Missing the ending quatation mark!");
                        throw 1;
                    }

                    return translateStringToToken();
                }

        }
    }

}

CTokenType::Token CLexer::translateStringToToken() {
    CTokenType::Token type = id_keyword_or_number();
    if (type != CTokenType::Token::UNKNOWN_TOKEN) {
        return type;
    }
    return CTokenType::Token::NAME;
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

string CLexer::lookAheadText() {
    return string(buf + charIndex, textLen);
}