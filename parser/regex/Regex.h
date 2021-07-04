//
// Created by 罗旭维 on 2021/7/3.
//

#ifndef REGEX_REGEX_H
#define REGEX_REGEX_H
#include <map>
#include <vector>
#include <string>
#include "LexerBuffer.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <algorithm>
#include "MacroLexer.h"


class Regex {
private:

    std::vector<std::string> regexExpr;
    MacroLexer* macroLexer;

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

public:
    Regex(const char * macroFilePath) {
        macroLexer = new MacroLexer(macroFilePath);
    }

    ~Regex() {
        delete macroLexer;
    }

    void printMacs() {
        macroLexer->printMacs();
    }


    void parse(const std::string& regex) {
        //扩展宏
        preProcessExpr(regex);

    }

    void printExpr() {
        printf("regular expression after expanded: \n");
        if (regexExpr.size() > 0) {
            std::for_each(regexExpr.begin(), regexExpr.end(), [](std::vector<std::string>::value_type item) {
                printf(item.c_str());
            });
        }
    }

};


#endif //REGEX_REGEX_H
