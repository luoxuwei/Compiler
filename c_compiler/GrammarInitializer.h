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
    typedef map<CTokenType::Token, vector<Production*>*> ProductionMap;
    static GrammarInitializer* getInstance();
    ProductionMap* getProductionMap();

    vector<Symbols*> * getSymbleArray() {return &symbolArray;}
    map<CTokenType::Token, Symbols*> * getSymbolMap() {return &symbolMap;}

    void initVariableDecalationProductions();
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
    void addProduction(Production &production, bool nullable);
    void addSymbolMapAndArray(Production &production, bool nullable);
    void addTerminalToSymbolMapAndArray();
};


#endif //CPARSE_GRAMMARINITIALIZER_H
