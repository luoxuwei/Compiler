//
// Created by 罗旭维 on 2021/7/4.
//

#include "ExprLexer.h"
void ExprLexer::preProcessExpr(const std::string &regex) {
    LexerBuffer lexer(regex.c_str(), regex.length());
    while (lexer.lookahead(1) != EOF) {
        //去空格换行符等空白字符
        while (isspace(lexer.lookahead(1))) {
            lexer.advance();
        }

        std::string expr;
        bool inquoted = false;
        char c = lexer.advance();
        while (!isspace(c) && c != EOF) {
            if (c == '"') {
                inquoted = !inquoted;
            }

            if (!inquoted && c == '{') {
                expr.append(expandMacro(extracMacroNameFromInput(lexer)));
            } else {
                expr.push_back(c);
            }

            c = lexer.advance();
        }

        //去空格换行符等空白字符
        while (isspace(lexer.lookahead(1))) {
            lexer.advance();
        }

        regexExpr.push_back(expr);
    }
}

std::string & ExprLexer::extracMacroNameFromInput(LexerBuffer &lexer) {
    char c = lexer.advance();
    std::string name;
    while (c != '}' && !isspace(c)) {
        name.push_back(c);
        c = lexer.advance();
    }

    if (c != '}') {
        parseErr(Error::E_BADMAC);
    }
    return name;
}

std::string ExprLexer::expandMacro(const std::string &name) {
    std::string content = macroLexer->expandMacro(name);
    int begin = content.find('{');
    int end = std::string::npos;
    std::string temp;
    while (begin != std::string::npos) {
        end = content.find('}', begin+1);
        if (end == std::string::npos) {
            parseErr(Error::E_BADMAC);//
        }
        bool inQuoted = checkInQuoted(content, begin, end);
        if (!inQuoted) {
            std::string curContent = macroLexer->expandMacro(content.substr(begin + 1, end - begin -1));
            content.replace(begin, end - begin + 1, curContent);
            //当前遇到的宏展开，可能有嵌套，继续找'{'
            begin = content.find('{');
        } else {
            //end之前的内容已确认不是宏, 继续往后找'{'
            begin = content.find('{', end);
        }
    }
    return content;
}

bool ExprLexer::checkInQuoted(std::string &macroContent, int curlyBracesBegin, int curlyBracesEnd) {
    int begin = macroContent.find('"');
    bool inQuoted = false;
    int end = std::string::npos;
    while (begin != std::string::npos) {
        end = macroContent.find('"', begin + 1);
        if (end == std::string::npos) {
            parseErr(Error::E_BADMAC);
        }
        //"{}"
        if (curlyBracesBegin > begin && curlyBracesEnd < end) {
            inQuoted = true;
        } else if (curlyBracesBegin > begin && curlyBracesEnd > end) {
            //"{"} 大括号不匹配
            parseErr(Error::E_BADMAC);
        } else if (curlyBracesBegin < begin && curlyBracesEnd < end) {
            //{"}" 大括号不匹配
            parseErr(Error::E_BADMAC);
        }
        begin = macroContent.find('"', end+1);
    }
    return inQuoted;
}