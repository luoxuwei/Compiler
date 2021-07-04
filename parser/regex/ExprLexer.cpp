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

ExprLexer::Token ExprLexer::advance() {
    if (currentToken == Token::EOS) {
        //一个正则表达式解析结束后读入下一个表达式
        if (exprCount >= regexExpr.size()) {
            currentToken = Token::END_OF_INPUT;
            return currentToken;
        } else {
            curExpr = &regexExpr.at(exprCount);
            exprCount++;
        }
    }

    if (charIndex >= curExpr->length()) {
        currentToken = Token::EOS;
        charIndex = 0;
        return currentToken;
    }

    char c = curExpr->at(charIndex);
    if (c == '"') {
        inQuoted = !inQuoted;
        charIndex++;
        c = curExpr->at(charIndex);
    }

    sawEsc = (c == '\\');
    if (sawEsc && curExpr->at(charIndex + 1) != '"' && !inQuoted) {
        lexeme = handleEsc();
    } else {
        if (sawEsc && curExpr->at(charIndex + 1) == '"') {
            lexeme = '"';
            charIndex += 2;
        } else {
            lexeme = c;
            charIndex++;
        }
    }

    currentToken = (inQuoted || sawEsc)? Token::L : tokenMap[c];
}

int ExprLexer::handleEsc() {
    /*当转移符 \ 存在时，它必须与跟在它后面的字符或字符串一起解读
     *我们处理的转义字符有以下几种形式
     * \b backspace
     * \f formfeed
     * \n newline
     * \r carriage return 回车
     * \s space 空格
     * \t tab
     * \e ASCII ESC ('\033')
     * \DDD 3位八进制数
     * \xDDD 3位十六进制数
     * \^C C是任何字符， 例如^A, ^B 在Ascii 表中都有对应的特殊含义
     * ASCII 字符表参见：
     * http://baike.baidu.com/pic/%E7%BE%8E%E5%9B%BD%E4%BF%A1%E6%81%AF%E4%BA%A4%E6%8D%A2%E6%A0%87%E5%87%86%E4%BB%A3%E7%A0%81/8950990/0/9213b07eca8065387d4c671896dda144ad348213?fr=lemma&ct=single#aid=0&pic=9213b07eca8065387d4c671896dda144ad348213
     */

    //跳过'\'
    charIndex++;
    char c = std::toupper(curExpr->at(charIndex));
    int rval = 0;
    switch (c) {
        case '\0' :
            rval = '\\';
            break;
        case 'B':
            rval = '\b';
            break;
        case 'F':
            rval = '\f';
            break;
        case 'N' :
            rval = '\n';
            break;
        case 'R' :
            rval = '\r';
            break;
        case 'S':
            rval = ' ';
            break;
        case 'T':
            rval = '\t';
            break;
        case 'E' :
            rval = '\033';
            break;
        case '^':
            charIndex++;
            /*
             * 因此当遇到^后面跟在一个字母时，表示读入的是控制字符
             * ^@ 在ASCII 表中的数值为0，^A 为1, 字符@在ASCII 表中数值为80， 字符A在ASCII表中数值为81
             * 'A' - '@' 等于1 就对应 ^A 在 ASCII 表中的位置
             * 具体可参看注释给出的ASCII 图
             *
             */
            rval = (char) (std::toupper(curExpr->at(charIndex)) - '@');
            break;
        case 'X':
             /*
              * \X 表示后面跟着的三个字符表示八进制或十六进制数
              */
            charIndex++; //越过X
            c = curExpr->at(charIndex);
            if (isHexDigit(c)) {
                rval = hex2Bin(c);
                charIndex++;
                c = curExpr->at(charIndex);
            }

            if (isHexDigit(c)) {
                rval << 4;
                rval += hex2Bin(c);
                charIndex++;
                c = curExpr->at(charIndex);
            }

            if (isHexDigit(c)) {
                rval << 4;
                rval += hex2Bin(c);
            }
            break;
        default:
            if (isOctDigit(c)) {
               rval = oct2Bin(curExpr->at(charIndex));
               charIndex++;
               if (isOctDigit(curExpr->at(charIndex))) {
                   rval << 3;
                   rval += oct2Bin(curExpr->at(charIndex));
                   charIndex++;
               }
                if (isOctDigit(curExpr->at(charIndex))) {
                    rval << 3;
                    rval += oct2Bin(curExpr->at(charIndex));
                }
            } else {
                rval = c;
            }


    }
    charIndex++;
    return rval;

}