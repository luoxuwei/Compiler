//
// Created by 罗旭维 on 2021/7/4.
//

#include "MacroLexer.h"
#include "util.h"

MacroLexer::MacroLexer(const char * macroFilePath) {
    if (macroFilePath != NULL) {
        LexerBuffer lexer(macroFilePath);
        processMacro(lexer);
    }
}

void MacroLexer::processMacro(LexerBuffer &lexer) {
    while (lexer.lookahead(1) != EOF) {
        //去掉开头的空格或空行
        while (isspace(lexer.lookahead(1))) {
            lexer.advance();
        }

        std::string macroName;
        char c = lexer.lookahead(1);
        while (!isspace(c)) {
            macroName.push_back(c);
            lexer.advance();
            c = lexer.lookahead(1);
        }

        //忽略宏定义名称后面的空格
        while (isspace(lexer.lookahead(1))) {
            lexer.advance();
        }

        //获取宏定义的内容
        c = lexer.lookahead(1);
        std::string macroContent;
        while (!isspace(c)) {
            macroContent.push_back(c);
            lexer.advance();
            c = lexer.lookahead(1);
        }

        //将宏定义加入哈希表
        macroMap[macroName] = macroContent;

        while (isspace(lexer.lookahead(1))) {
            lexer.advance();
        }
    }
}

std::string MacroLexer::expandMacro(const std::string &name) {
    if (macroMap.find(name) == macroMap.end()) {
        parseErr(Error::E_NOMAC);
    } else {
        std::string macro(macroMap[name]);
        macro.insert(0, "(");
        macro.append(")");
        return macro;
    }
}