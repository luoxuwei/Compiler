//
// Created by 罗旭维 on 2021/10/6.
//

#ifndef CPARSE_GRAMMARINITIALIZER_H
#define CPARSE_GRAMMARINITIALIZER_H
#include "CTokenType.h"
#include "Production.h"
#include "Symbols.h"
#include <map>
#include <vector>

using namespace std;
class GrammarInitializer {

public:
    const static int TYPE_TO_TYPE_SPECIFIER = 11;
    const static int SPECIFIERS_TypeOrClass_TO_SPECIFIERS = 9;
    const static int NAME_TO_NewName = 12;
    const static int START_VarDecl_TO_VarDecl = 14;
    const static int ExtDeclList_COMMA_ExtDecl_TO_ExtDeclList = 5;
    const static int OptSpecifier_ExtDeclList_Semi_TO_ExtDef = 2;
    const static int TypeNT_VarDecl_TO_ParamDeclaration = 20;
    const static int VarList_COMMA_ParamDeclaration_TO_VarList = 19;

    const static int NewName_LP_VarList_RP_TO_FunctDecl = 16;
    const static int NewName_LP_RP_TO_FunctDecl = 17;

    const static int Start_VarDecl_TO_VarDecl = 39;
    const static int DeclList_Comma_Decl_TO_DeclList = 33;
    const static int Specifiers_DeclList_Semi_TO_Def = 30;
    const static int DefList_Def_TO_DefList = 29;
    const static int Name_To_Tag = 27;
    const static int Struct_OptTag_LC_DefList_RC_TO_StructSpecifier = 24;
    const static int StructSpecifier_TO_TypeSpecifier = 23;

    const static int Enum_TO_EnumNT = 41;
    const static int NameNT_TO_Emurator = 44;
    const static int Name_TO_NameNT = 45;
    const static int Name_Eequal_ConstExpr_TO_Enuerator = 46;
    const static int Number_TO_ConstExpr = 47;
    const static int EnumSpecifier_TO_TypeSpecifier = 49;

    const static int NoCommaExpr_TO_Expr = 53;
    const static int NoCommaExpr_Equal_NoCommaExpr_TO_NoCommaExpr = 54;
    const static int Binary_TO_NoCommaExpr = 56;
    const static int Uanry_TO_Binary = 57;
    const static int Number_TO_Unary = 58;
    const static int Name_TO_Unary = 59;
    const static int String_TO_Unary = 60;
    //STATEMENT -> EXPR SEMI(63)
    const static int  Expr_Semi_TO_Statement = 63;
    //STMT_LIST ->  STATEMENT(62)
    const static int  Statement_TO_StmtList = 62;
    //STMT_LIST -> STMT_LIST STATEMENT(61)
    const static int  StmtList_Statement_TO_StmtList = 61;

    const static int Binary_Plus_Binary_TO_Binary = 94;

    //STATEMENT -> LOCAL_DEFS(68)
    const static int LocalDefs_TO_Statement = 68;

    //VAR_DECL -> VAR_DECL LB CONST_EXPR RB  a[5] (109)
    const static int VarDecl_LB_ConstExpr_RB_TO_VarDecl = 109;

    //UNARY -> UNARY LB EXPR RB (110)
    const static int Unary_LB_Expr_RB_TO_Unary = 101;

    //TEST -> EXPR 76
    const static int Expr_TO_Test= 76;
    //IF_STATEMENT -> IF LP TEST RP STATEMENT (72)
    const static int If_Test_Statement_TO_IFStatement = 72;
    //BINARY -> BINARY RELOP BINARY (65)
    const static int Binary_RelOP_Binary_TO_Binray = 65;
    //IF_ELSE_STATEMENT ->IF_ELSE_STATEMENT ELSE STATEMENT 74
    const static int IfElseStatemnt_Else_Statemenet_TO_IfElseStatement = 74;
    //OPT_EXPR -> EXPR SEMI(85)
    const static int  Expr_Semi_TO_OptExpr = 85;
    //OPT_EXPR -> SEMI(86)
    const static int Semi_TO_OptExpr = 86;
    //END_OPT_EXPR -> EXPR(87)
    const static int Expr_TO_EndOpt = 87;
    //STATEMENT -> FOR LP OPT_EXPR  TEST SEMI END_OPT_EXPR RP STATEMENT(84)
    const static int FOR_OptExpr_Test_EndOptExpr_Statement_TO_Statement = 84;

    //STATEMENT -> WHILE LP TEST RP STATEMENT (83)
    const static int While_LP_Test_Rp_TO_Statement = 83;
    //STATEMENT -> DO STATEMENT WHILE LP TEST RP SEMI(88)
    const static int Do_Statement_While_Test_To_Statement = 88;

    //UNARY -> UNARY INCOP i++ (96)
    const static int Unary_Incop_TO_Unary = 96;
    //UNARY -> INCOP UNARY ++i (97)
    const static int Incop_Unary_TO_Unary = 97;
    //UNARY -> MINUS UNARY
    const static int Minus_Unary_TO_Unary = 98;

    //STATEMENT -> COMPOUND_STMT (71)
    const static int CompountStmt_TO_Statement = 71;
    //DECL -> VAR_DECL EQUAL INITIALIZER 77
    const static int VarDecl_Equal_Initializer_TO_Decl = 77;
    //INITIALIZER -> EXPR 78
    const static int Expr_TO_Initializer = 78;
    //LOCAL_DEFS -> DEF_LIST(52)
    const static int DefList_TO_LocalDefs = 52;
    //VAR_DECL ->  NEW_NAME(13)
    const static int NewName_TO_VarDecl = 13;
    //COMPOUND_STMT-> LC LOCAL_DEFS STMT_LIST RC(51)
    const static int LocalDefs_StmtList_TO_CompoundStmt = 51;
    //EXT_DEF -> OPT_SPECIFIERS FUNCT_DECL COMPOUND_STMT(50)
    const static int OptSpecifiers_FunctDecl_CompoundStmt_TO_ExtDef = 50;
    //UNARY -> UNARY LP RP  fun()
    const static int Unary_LP_RP_TO_Unary = 103;
    //UNARY -> UNARY LP ARGS RP fun(a, b ,c)
    const static int Unary_LP_ARGS_RP_TO_Unary = 102;
    //ARGS -> NO_COMMA_EXPR  (104)
    const static int NoCommaExpr_TO_Args = 104;
    //ARGS -> NO_COMMA_EXPR COMMA ARGS (105)
    const static int NoCommaExpr_Comma_Args_TO_Args = 105;

    //STATEMENT -> RETURN SEMI (111)
    const static int Return_Semi_TO_Statement = 111;
    //STATEMENT -> RETURN EXPR SEMI (64)
    const static int Return_Expr_Semi_TO_Statement = 64;

    //UNARY -> LP EXPR RP (112)
    const static int LP_Expr_RP_TO_Unary = 112;
    //UNARY -> UNARY DECOP i--
    const static int Unary_DecOp_TO_Unary = 113;
    //BINARY -> BINARY DIVOP BINARY(90)
    const static int Binary_DivOp_Binary_TO_Binary = 90;
    //BINARY -> BINARY MINUS BINARY(95)
    const static int Binary_Minus_Binary_TO_Binary = 95;
    //UNARY -> STAR UNARY 99
    const static int Start_Unary_TO_Unary = 99;

    typedef map<CTokenType::Token, vector<Production*>*> ProductionMap;
    static GrammarInitializer* getInstance();
    ProductionMap* getProductionMap();

    vector<Symbols*> * getSymbleArray() {return &symbolArray;}
    map<CTokenType::Token, Symbols*> * getSymbolMap() {return &symbolMap;}

    void initVariableDecalationProductions();
    void initFunctionProductions();
    void initStructureProductions();
    void initEmunProductions();
    void initFunctionDefinition();
    void initFunctionDefinition2();
    void initFunctionDefinitionWithIfElse();
    void initFunctionDefinitionWithSwitchCase();
    void initFunctionDefinitionWithLoop();
    void initComputingOperation();
    void initRemaindingProduction();
    void initProductions();
    struct ProductionComparator {
        Production &value;
        ProductionComparator(Production &production):value(production) {}
        bool operator()(Production * production) {
            return value == *production;
        }
    };

private:
    static GrammarInitializer* instance;
    ProductionMap productionMap;
    vector<Symbols*> symbolArray;
    map<CTokenType::Token, Symbols*> symbolMap;
    int productionNum = 0;
    GrammarInitializer();
    void addProduction(Production &production, bool nullable);
    void addSymbolMapAndArray(Production &production, bool nullable);
    void addTerminalToSymbolMapAndArray();
};


#endif //CPARSE_GRAMMARINITIALIZER_H
