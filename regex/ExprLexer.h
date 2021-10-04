//
// Created by 罗旭维 on 2021/7/4.
//

#ifndef REGEX_EXPRLEXER_H
#define REGEX_EXPRLEXER_H
#include <vector>
#include "MacroLexer.h"
#define ASCII_COUNT 128
class ExprLexer {

public:
    enum Token {
        EOS, //正则表达式末尾
        ANY,     // . 通配符
        AT_BOL,  //^ 开头匹配符
        AT_EOL,  //$ 末尾匹配符
        CCL_END,  //字符集类结尾括号 ]
        CCL_START,  //字符集类开始括号 [
        CLOSE_CURLY, // }
        CLOSE_PAREN,  //)
        CLOSURE,      //*
        DASH,       // -
        END_OF_INPUT,  //输入流结束
        L,        //字符常量
        OPEN_CURLY, // {
        OPEN_PAREN, // (
        OPTIONAL,  //?
        OR,       // |
        PLUS_CLOSE
    };

    ExprLexer(const char * macroFilePath, const std::string& regex) {
        macroLexer = new MacroLexer(macroFilePath);
        //扩展宏
        preProcessExpr(regex);

        for (int i=0; i<ASCII_COUNT; i++) {
            tokenMap[i] = Token::L;
        }
        tokenMap['.'] = Token::ANY;
        tokenMap['^'] = Token::AT_BOL;
        tokenMap['$'] = Token::AT_EOL;
        tokenMap[']'] = Token::CCL_END;
        tokenMap['['] = Token::CCL_START;
        tokenMap['}'] = Token::CLOSE_CURLY;
        tokenMap[')'] = Token::CLOSE_PAREN;
        tokenMap['*'] = Token::CLOSURE;
        tokenMap['-'] = Token::DASH;
        tokenMap['{'] = Token::OPEN_CURLY;
        tokenMap['('] = Token::OPEN_PAREN;
        tokenMap['?'] = Token::OPTIONAL;
        tokenMap['|'] = Token::OR;
        tokenMap['+'] = Token::PLUS_CLOSE;
    }

    ~ExprLexer() {
        delete macroLexer;
    }

    bool matchToken(Token t) {
        return currentToken == t;
    }

    Token token() {
        return currentToken;
    }

    int getLexeme() {
        return lexeme;
    }

    Token advance();

    void printMacs() {
        macroLexer->printMacs();
    }

    void printExpr() {
        printf("regular expression after expanded: \n");
        if (regexExpr.size() > 0) {
            std::for_each(regexExpr.begin(), regexExpr.end(), [](std::vector<std::string>::value_type item) {
                printf(item.c_str());
            });
        }
    }

    int getRegularExpressionCount() {
        return regexExpr.size();
    }

    const char * getRegularExpression(int index) {
        if (index < 0 || index >= regexExpr.size()) {
            return NULL;
        }

        return regexExpr.at(index).c_str();
    }

private:
    std::vector<std::string> regexExpr;
    MacroLexer* macroLexer;
    Token currentToken = Token::EOS;
    int exprCount = 0;
    std::string *curExpr;
    int charIndex = 0;
    bool inQuoted = false; //是否在双引号内
    bool sawEsc = false;  //是否读取到转移符 /
    int lexeme ;
    Token tokenMap[ASCII_COUNT] = {Token::L};
    /*
     * 对正则表达式进行预处理，将表达式中的宏进行替换，例如
     * D*\.D 预处理后输出
     * [0-9]*\.[0-9]
     * 注意，宏是可以间套的，所以宏替换时要注意处理间套的情形
     */
    void preProcessExpr(const std::string& regex);
    std::string & extracMacroNameFromInput(LexerBuffer &lexer);
    std::string expandMacro(const std::string &name);
    bool checkInQuoted(std::string &macroContent, int curlyBracesBegin, int curlyBracesEnd);

    int handleEsc();
    bool isHexDigit(char c) {
        return (isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'));
    }

    bool isOctDigit(char c) {
        return ('0' <= c && c <= '7');
    }

    int hex2Bin(char c) {
        /*
        * 将十六进制数对应的字符转换为对应的数值，例如
        * A 转换为10， B转换为11
        * 字符c 必须满足十六进制字符： 0123456789ABCDEF
        */
        return (isdigit(c)? c - '0': (std::toupper(c) - 'A' + 10)) & 0xf;
    }

    int oct2Bin(char c) {
        /*
         * 将字符c 转换为对应的八进制数
         * 字符c 必须是合法的八进制字符: 01234567
         */
        return ((c) - '0') & 0x7;
    }


};


#endif //REGEX_EXPRLEXER_H
