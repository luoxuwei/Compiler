//
// Created by 罗旭维 on 2021/10/7.
//

#ifndef CPARSE_FIRSTSETBUILDER_H
#define CPARSE_FIRSTSETBUILDER_H
#include "GrammarInitializer.h"
#include "CTokenType.h"
#include "Symbols.h"
#include <map>
#include <vector>

class FirstSetBuilder {
private:
    map<CTokenType::Token, Symbols*> *symbolMap;
    vector<Symbols*> *symbolArray;
    bool runFirstSetPass = true;
    void addSymbolFirstSet(Symbols *symbols);
public:
    FirstSetBuilder();
    bool isSymbolNullable(CTokenType::Token token);
    void runFirstSets();
    vector<CTokenType::Token> *getFirstSet(CTokenType::Token symbol);
};


#endif //CPARSE_FIRSTSETBUILDER_H
