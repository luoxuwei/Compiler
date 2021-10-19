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

        TYPE_OR_CLASS, TYPE_NT,

        /*struct def*/
        STRUCT_SPECIFIER,OPT_TAG,DEF_LIST,TAG,DEF,DECL_LIST,DECL,

        FUNCT_DECL, VAR_LIST, PARAM_DECLARATION,

        //Enum
        ENUM_SPECIFIER, OPT_ENUM_LIST, ENUMERATOR_LIST, ENUMERATOR, ENUM_NT,

        CONST_EXPR,

        COMPOUND_STMT, LOCAL_DEFS, STMT_LIST, INITIALIZER, EXPR, NO_COMMA_EXPR,BINARY, UNARY, STATEMENT,

        TYPE_SPECIFIER, NEW_NAME, NAME_NT,



        //terminals
        NAME, TYPE, STRUCT,CLASS, LP, RP, LB, RB,PLUS,LC,RC,NUMBER, STRING, QUEST, COLON,
        RELOP, ANDAND, OR, AND, EQUOP, SHIFTOP, DIVOP,
        RETURN,

        //Enum def
        ENUM,

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

                stringify(STRUCT_SPECIFIER),
                stringify(OPT_TAG),
                stringify(DEF_LIST),
                stringify(TAG),
                stringify(DEF),
                stringify(DECL_LIST),
                stringify(DECL),

                stringify(FUNCT_DECL),
                stringify(VAR_LIST),
                stringify(PARAM_DECLARATION),

                stringify(ENUM_SPECIFIER),
                stringify(OPT_ENUM_LIST),
                stringify(ENUMERATOR_LIST),
                stringify(ENUMERATOR),
                stringify(ENUM_NT),
                stringify(CONST_EXPR),
                stringify(COMPOUND_STMT),
                stringify(LOCAL_DEFS),
                stringify(STMT_LIST),
                stringify(INITIALIZER),
                stringify(EXPR),
                stringify(NO_COMMA_EXPR),
                stringify(BINARY),
                stringify(UNARY),
                stringify(STATEMENT),

                stringify(TYPE_SPECIFIER),
                stringify(NEW_NAME),
                stringify(NAME_NT),
                stringify(NAME),
                stringify(TYPE),
                stringify(STRUCT),
                stringify(CLASS),
                stringify(LP),
                stringify(RP),
                stringify(LB),
                stringify(RB),

                stringify(PLUS),
                stringify(LC),
                stringify(RC),

                stringify(NUMBER),
                stringify(STRING),
                stringify(QUEST),
                stringify(COLON),
                stringify(RELOP),
                stringify(ANDAND),
                stringify(OR),
                stringify(AND),
                stringify(EQUOP),
                stringify(SHIFTOP),
                stringify(DIVOP),
                stringify(RETURN),
                stringify(ENUM),


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
