//
// Created by 罗旭维 on 2021/10/7.
//

#ifndef CPARSE_SYMBOLS_H
#define CPARSE_SYMBOLS_H
#include <vector>
#include "CTokenType.h"
#include "Production.h"
using namespace std;
class Symbols {
public:
    typedef vector<vector<CTokenType::Token>> ProductionList;
    CTokenType::Token value;
    ProductionList *productions;
    vector<CTokenType::Token> firstSet;
    bool nullable;
    Symbols(CTokenType::Token symVal, bool nullable, ProductionList *productions);
    void addProduction(vector<CTokenType::Token> production);
};


#endif //CPARSE_SYMBOLS_H
