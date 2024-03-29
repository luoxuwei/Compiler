//
// Created by 罗旭维 on 2021/9/28.
//

#ifndef CPARSE_CTOKENTYPE_H
#define CPARSE_CTOKENTYPE_H
#define stringify(token) # token
class CTokenType {
public:
    enum Token {
//non-termals

//        PROGRAM, EXT_DEF_LIST, EXT_DEF, OPT_SPECIFIERS, EXT_DECL_LIST,
//        EXT_DECL, VAR_DECL, SPECIFIERS,

        	stmt, expr, term, factor,


//        TYPE_OR_CLASS, TYPE_NT,
//        TYPE_SPECIFIER, NEW_NAME, NAME_NT,


        //terminals
        NUMBER, STAR, LP, RP, PLUS, SEMI, NAME, TYPE, CLASS, LB, RB,

//	NUM, TIMES,

        COMMA, WHITE_SPACE, EQUAL, TTYPE, UNKNOWN_TOKEN
    };

    static const int FIRST_TERMINAL_INDEX = NAME;
    static const int LAST_TERMINAL_INDEX = UNKNOWN_TOKEN;
    static bool isTerminal(CTokenType::Token token);
    static const char * getSymbolStr(CTokenType::Token token) {
        const static char *symbolStr[] = {
                stringify(stmt),
                stringify(expr),
                stringify(term),
                stringify(factor),
                stringify(NUMBER),
                stringify(STAR),
                stringify(LP),
                stringify(RP),
                stringify(PLUS),
                stringify(SEMI)
        };
        if (token > SEMI) return "";
        return symbolStr[token];
    }
};
#endif //CPARSE_CTOKENTYPE_H
