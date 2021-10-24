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
