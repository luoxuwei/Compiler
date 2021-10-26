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
    keywordMap["if"] = CTokenType::Token::IF;
    keywordMap["else"] = CTokenType::Token::ELSE;
    keywordMap["switch"] = CTokenType::Token::SWITCH;
    keywordMap["case"] = CTokenType::Token::CASE;
    keywordMap["default"] = CTokenType::Token::DEFAULT;
    keywordMap["break"] = CTokenType::Token::BREAK;
    keywordMap["for"] = CTokenType::Token::FOR;
    keywordMap["while"] = CTokenType::Token::WHILE;
    keywordMap["do"] = CTokenType::Token::DO;
    keywordMap["goto"] = CTokenType::Token::GOTO;
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
    if (isalpha(buf[begin])) {
        string text(buf + begin, textLen);
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

    if (charIndex >= buf_len) return CTokenType::Token::SEMI;

    textLen = 0;
    int j = 0;
    for (int i = charIndex; i < buf_len; i++) {
        switch (buf[charIndex]) {
            case ';':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::SEMI;
            case '+':
                if (buf[charIndex + 1] == '+') {
                    textLen = 2;
                    return CTokenType::Token::INCOP;
                }
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::PLUS;
            case '-':
                if (buf[charIndex + 1] == '>') {
                    textLen = 2;
                    return CTokenType::Token::STRUCTOP;
                } else if (buf[charIndex + 1] == '-') {
                    textLen = 2;
                    return CTokenType::Token::INCOP;
                }
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::MINUS;
            case '[':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::LB;
            case ']':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::RB;
            case '*':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::STAR;
            case '(':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::LP;
            case ')':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::RP;
            case ',':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::COMMA;
            case '{':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::LC;
            case '}':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::RC;
            case '=':
                if (buf[charIndex + 1] == '=') {
                    textLen = 2;
                    return CTokenType::Token::RELOP;
                }
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::EQUAL;
            case '?':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::QUEST;
            case ':':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::COLON;
            case '&':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::AND;
            case '|':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::OR;
            case '^':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::XOR;
            case '/':
            case '%':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::DIVOP;
            case '>':
            case '<':
                begin = charIndex;
                if (buf[i + 1] == '=') {
                    textLen = 2;
                } else if ((buf[i] == '<' && buf[i + 1] == '<') || (buf[i] == '>' && buf[i + 1] == '>')) {
                    textLen = 2;
                    return CTokenType::Token::SHIFTOP;
                } else {
                    textLen = 1;
                }
                charIndex = charIndex + textLen;
                return CTokenType::Token::RELOP;
            case '\n':
            case '\t':
            case ' ':
                begin = charIndex;
                textLen = 1;
                charIndex = charIndex + textLen;
                return CTokenType::Token::WHITE_SPACE;
            case '"':
                begin = charIndex + 1;
                j = begin;
                while (j < buf_len) {
                    if (buf[j] != '"') {
                        textLen++;
                    } else {
                        break;
                    }
                    j++;
                }
                if (j >= buf_len) {
                    printf("Missing the ending quatation mark!");
                    throw 1;
                }
                charIndex = charIndex + textLen + 2;
                return CTokenType::Token::STRING;
            default:
                if (!isalnum(buf[charIndex])) {
                    return CTokenType::Token::UNKNOWN_TOKEN;
                }
                else {
                    while (i < buf_len && isalnum(buf[i])) {
                        i++;
                        textLen++;
                    }
                    begin = charIndex;
                    charIndex = charIndex + textLen;
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
    if (buf[begin] == '-' || buf[begin] == '+') {
        i++;
    }

    for (; i<textLen; i++) {
        if (!isdigit(buf[begin+i])) return false;
    }
    return true;
}

string CLexer::lookAheadText() {
    return string(buf + begin, textLen);
}