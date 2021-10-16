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

    typedef map<CTokenType::Token, vector<Production*>*> ProductionMap;
    static GrammarInitializer* getInstance();
    ProductionMap* getProductionMap();

    vector<Symbols*> * getSymbleArray() {return &symbolArray;}
    map<CTokenType::Token, Symbols*> * getSymbolMap() {return &symbolMap;}

    void initVariableDecalationProductions();
    void initFunctionProductions();
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
