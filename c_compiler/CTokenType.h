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

        PROGRAM, EXT_DEF_LIST, EXT_DEF, OPT_SPECIFIERS, EXT_DECL_LIST,
        EXT_DECL, VAR_DECL, SPECIFIERS,

        //	stmt, expr, term, factor,


        TYPE_OR_CLASS, TYPE_NT,
        FUNCT_DECL, VAR_LIST, PARAM_DECLARATION,
        TYPE_SPECIFIER, NEW_NAME, NAME_NT,



        //terminals
        NAME, TYPE, CLASS, LP, RP, LB, RB,PLUS,

//	NUM, TIMES,

        COMMA, SEMI, WHITE_SPACE, EQUAL,TTYPE, STAR, UNKNOWN_TOKEN
    };

    static const int FIRST_TERMINAL_INDEX = NAME;
    static const int LAST_TERMINAL_INDEX = UNKNOWN_TOKEN;
    static bool isTerminal(CTokenType::Token token);
    static const char * getSymbolStr(CTokenType::Token token) {
        const static char *symbolStr[] = {
                stringify(PROGRAM),
                stringify(EXT_DEF_LIST),
                stringify(EXT_DEF),
                stringify(OPT_SPECIFIERS),
                stringify(EXT_DECL_LIST),
                stringify(EXT_DECL),
                stringify(VAR_DECL),
                stringify(SPECIFIERS),
                stringify(TYPE_OR_CLASS),
                stringify(TYPE_NT),

                stringify(FUNCT_DECL),
                stringify(VAR_LIST),
                stringify(PARAM_DECLARATION),


                stringify(TYPE_SPECIFIER),
                stringify(NEW_NAME),
                stringify(NAME_NT),
                stringify(NAME),
                stringify(TYPE),
                stringify(CLASS),
                stringify(LP),
                stringify(RP),
                stringify(LB),
                stringify(RB),

                stringify(PLUS),
                stringify(COMMA),
                stringify(SEMI),
                stringify(WHITE_SPACE),
                stringify(EQUAL),
                stringify(TTYPE),
                stringify(STAR),
                stringify(UNKNOWN_TOKEN),
        };
        if (token > UNKNOWN_TOKEN) return "";
        return symbolStr[token];
    }
};
#endif //CPARSE_CTOKENTYPE_H
