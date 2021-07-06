#include <iostream>
#include "ExprLexer.h"
#include "Regex.h"
void printLexResult(ExprLexer &lexer);
void printMetaCharMeaning(ExprLexer &lexer);


int main(int argc, char** argv) {
    if (argc > 2) {
        ExprLexer lexer(argv[1], argv[2]);
        lexer.printMacs();
        lexer.printExpr();

        int exprCount = 0;
        printf("\n");
        printf("当前正则解析的正则表达式: %s \n", lexer.getRegularExpression(exprCount));
        lexer.advance();

        while (!lexer.matchToken(ExprLexer::Token::END_OF_INPUT)) {

            if (lexer.matchToken(ExprLexer::Token::EOS)) {
                printf("解析下一个正则表达式 \n");
                exprCount++;
                printf("当前正则解析的正则表达式: ", lexer.getRegularExpression(exprCount));
                lexer.advance();
            }
            else {
                printLexResult(lexer);
            }

        }

        Regex regex(argv[1], argv[2]);
        regex.cat_expr();
        regex.printNfa();
    }
    return 0;
}

void printLexResult(ExprLexer &lexer) {
    while (!lexer.matchToken(ExprLexer::Token::EOS)) {
        printf("当前识别字符是:%c \n", (char)lexer.getLexeme());

        if (!lexer.matchToken(ExprLexer::Token::L)) {
            printf("当前字符具有特殊含义 \n");

            printMetaCharMeaning(lexer);
        }
        else {
            printf("当前字符是普通字符常量 \n");
        }

        lexer.advance();
    }
}

void printMetaCharMeaning(ExprLexer &lexer) {
    std::string s = "";
    if (lexer.matchToken(ExprLexer::Token::ANY)) {
        s = "当前字符是点通配符";
    }

    if (lexer.matchToken(ExprLexer::Token::AT_BOL)) {
        s = "当前字符是开头匹配符";
    }

    if (lexer.matchToken(ExprLexer::Token::AT_EOL)) {
        s = "当前字符是末尾匹配符";
    }

    if (lexer.matchToken(ExprLexer::Token::CCL_END)) {
        s = "当前字符是字符集类结尾括号";
    }

    if (lexer.matchToken(ExprLexer::Token::CCL_START)) {
        s = "当前字符是字符集类的开始括号";
    }

    if (lexer.matchToken(ExprLexer::Token::CLOSE_CURLY)) {
        s = "当前字符是结尾大括号";
    }

    if (lexer.matchToken(ExprLexer::Token::CLOSE_PAREN)) {
        s = "当前字符是结尾圆括号";
    }

    if (lexer.matchToken(ExprLexer::Token::DASH)) {
        s = "当前字符是横杆";
    }

    if (lexer.matchToken(ExprLexer::Token::OPEN_CURLY)) {
        s = "当前字符是起始大括号";
    }

    if (lexer.matchToken(ExprLexer::Token::OPEN_PAREN)) {
        s = "当前字符是起始圆括号";
    }

    if (lexer.matchToken(ExprLexer::Token::OPTIONAL)) {
        s = "当前字符是单字符匹配符?";
    }

    if (lexer.matchToken(ExprLexer::Token::OR)) {
        s = "当前字符是或操作符";
    }

    if (lexer.matchToken(ExprLexer::Token::PLUS_CLOSE)) {
        s = "当前字符是正闭包操作符";
    }

    if (lexer.matchToken(ExprLexer::Token::CLOSURE)) {
        s = "当前字符是闭包操作符";
    }

    printf(s.c_str());
    printf("\n");
}
