//
// Created by 罗旭维 on 2021/9/28.
//

#ifndef CPARSE_CTOKENTYPE_H
#define CPARSE_CTOKENTYPE_H

class CTokenType {
    enum Token {
//non-termals

        PROGRAM, EXT_DEF_LIST, EXT_DEF, OPT_SPECIFIERS, EXT_DECL_LIST,
        EXT_DECL, VAR_DECL, SPECIFIERS,

        //	stmt, expr, term, factor,


        TYPE_OR_CLASS, TYPE_NT,
        TYPE_SPECIFIER, NEW_NAME, NAME_NT,


        //terminals
        NAME, TYPE, CLASS, LP, RP, LB, RB, PLUS,

//	NUM, TIMES,

        COMMA, SEMI, WHITE_SPACE, EQUAL, TTYPE, STAR, UNKNOWN_TOKEN
    };

    static const int FIRST_TERMINAL_INDEX = NAME;
    static const int LAST_TERMINAL_INDEX = UNKNOWN_TOKEN;
    bool isTerminal(CTokenType::Token token);
};
#endif //CPARSE_CTOKENTYPE_H
