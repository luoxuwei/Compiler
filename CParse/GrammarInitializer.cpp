//
// Created by 罗旭维 on 2021/10/6.
//

#include "GrammarInitializer.h"
GrammarInitializer* GrammarInitializer::instance = NULL;
GrammarInitializer * GrammarInitializer::getInstance() {
    if (instance == NULL) {
        instance = new GrammarInitializer();
    }
    return instance;
}

GrammarInitializer::ProductionMap* GrammarInitializer::getProductionMap() {
    return &productionMap;
}

void GrammarInitializer::initStmtForDebug() {
    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //stmt -> expr
    right = new vector<CTokenType::Token>({CTokenType::Token::expr});//getProductionRight(productionRight);
    production = new Production(CTokenType::Token::stmt, 0, *right);
    addProduction(*production);

    //expr -> expr + term
    right = new vector<CTokenType::Token>({CTokenType::Token::expr, CTokenType::Token::PLUS, CTokenType::Token::term});
    production = new Production(CTokenType::expr, 0, *right);
    addProduction(*production);

    //expr -> term
    right = new vector<CTokenType::Token>({ CTokenType::Token::term});
    production = new Production(CTokenType::Token::expr, 0, *right);
    addProduction(*production);

    //term -> term * factor
    right = new vector<CTokenType::Token>({CTokenType::Token::term, CTokenType::Token::STAR, CTokenType::Token::factor});
    production = new Production(CTokenType::Token::term, 0, *right);
    addProduction(*production);

    //term -> factor
    right = new vector<CTokenType::Token>({CTokenType::Token::factor});
    production = new Production(CTokenType::Token::term, 0, *right);
    addProduction(*production);

    //factor -> ( expr )
    right = new vector<CTokenType::Token>({CTokenType::Token::LP, CTokenType::Token::expr, CTokenType::Token::RP});
    production = new Production(CTokenType::Token::factor, 0, *right);
    addProduction(*production);

    //factor -> num
    right = new vector<CTokenType::Token>({CTokenType::Token::NUMBER});
    production = new Production(CTokenType::Token::factor, 0, *right);
    addProduction(*production);
}

void GrammarInitializer::addProduction(Production &production) {
    vector<Production *>* productionList = NULL;
    auto iter = productionMap.find(production.getLeft());
    if (iter == productionMap.end()) {
        productionList = new vector<Production *>();
        productionMap[production.getLeft()] = productionList;
    } else {
        productionList = iter->second;
    }

    auto listIter = find_if(productionList->begin(), productionList->end(), ProductionComparator(production));
    if (listIter == productionList->end()) {
        productionList->push_back(&production);
    }
    addSymbolMapAndArray(production, false);
}

void GrammarInitializer::addSymbolMapAndArray(Production &production, bool nullable) {
    vector<CTokenType::Token> &right = production.getRight();

    auto iter = symbolMap.find(production.getLeft());
    if (iter != symbolMap.end()) {
        symbolMap[production.getLeft()]->addProduction(right);
    } else {
        Symbols::ProductionList *productionList = new Symbols::ProductionList();
        productionList->push_back(right);
        Symbols *symbols = new Symbols(production.getLeft(), nullable, productionList);
        symbolMap[production.getLeft()] = symbols;
        symbolArray.push_back(symbols);
    }
}