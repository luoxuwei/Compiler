//
// Created by 罗旭维 on 2021/7/4.
//

#ifndef REGEX_EXPRLEXER_H
#define REGEX_EXPRLEXER_H
#include <vector>
#include "MacroLexer.h"

class ExprLexer {
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
    enum Type {aaa, bbb};

    ExprLexer(const char * macroFilePath, const std::string& regex) {
        macroLexer = new MacroLexer(macroFilePath);
        //扩展宏
        preProcessExpr(regex);
    }

    ~ExprLexer() {
        delete macroLexer;
    }

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
};


#endif //REGEX_EXPRLEXER_H
