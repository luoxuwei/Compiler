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

void GrammarInitializer::initVariableDecalationProductions() {
    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    /*
    	 *
    	 * C variable declaration grammar
    	 *  PROGRAM -> EXT_DEF_LIST
    	 *
    	 *  EXT_DEF_LIST -> EXT_DEF_LIST EXT_DEF
    	 *
    	 *  EXT_DEF -> OPT_SPECIFIERS EXT_DECL_LIST  SEMI
    	 *             | OPT_SPECIFIERS SEMI
    	 *
    	 *
    	 *  EXT_DECL_LIST ->   EXT_DECL
    	 *                   | EXT_DECL_LIST COMMA EXT_DECL
    	 *
    	 *  EXT_DECL -> VAR_DECL
    	 *
    	 *  OPT_SPECIFIERS -> CLASS TTYPE
    	 *                   | TTYPE
    	 *                   | SPECIFIERS
    	 *                   | EMPTY?
    	 *
    	 *  SPECIFIERS -> TYPE_OR_CLASS
    	 *                | SPECIFIERS TYPE_OR_CLASS
    	 *
    	 *
    	 *  TYPE_OR_CLASS -> TYPE_SPECIFIER
    	 *                   | CLASS
    	 *
    	 *  TYPE_SPECIFIER ->  TYPE
    	 *
    	 *  NEW_NAME -> NAME
    	 *
    	 *  NAME_NT -> NAME
    	 *
    	 *  VAR_DECL -> | NEW_NAME
    	 *
    	 *              | START VAR_DECL
    	 *
    	 */
    int productionNum = 0;
    //PROGRAM -> EXT_DEF_LIST
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DEF_LIST});//getProductionRight(productionRight);
    production = new Production(productionNum, CTokenType::Token::PROGRAM, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //EXT_DEF_LIST -> EXT_DEF_LIST EXT_DEF
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DEF_LIST, CTokenType::Token::EXT_DEF});
    production = new Production(productionNum, CTokenType::EXT_DEF_LIST, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //EXT_DEF -> OPT_SPECIFIERS EXT_DECL_LIST  SEMI
    right = new vector<CTokenType::Token>({ CTokenType::Token::OPT_SPECIFIERS, CTokenType::Token::EXT_DECL_LIST, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::EXT_DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXT_DEF -> OPT_SPECIFIERS  SEMI
    right = new vector<CTokenType::Token>({ CTokenType::Token::OPT_SPECIFIERS, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::EXT_DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXT_DECL_LIST ->   EXT_DECL
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DECL});
    production = new Production(productionNum, CTokenType::Token::EXT_DECL_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    ///EXT_DECL_LIST ->EXT_DECL_LIST COMMA EXT_DECL
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DECL_LIST, CTokenType::Token::COMMA, CTokenType::Token::EXT_DECL});
    production = new Production(productionNum, CTokenType::Token::EXT_DECL_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXT_DECL -> VAR_DECL
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL});
    production = new Production(productionNum, CTokenType::Token::EXT_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

/*    //OPT_SPECIFIERS -> CLASS TTYPE
    right = new vector<CTokenType::Token>({CTokenType::Token::CLASS, CTokenType::Token::TTYPE});
    production = new Production(productionNum, CTokenType::Token::OPT_SPECIFIERS, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //OPT_SPECIFIERS -> TTYPE
    right = new vector<CTokenType::Token>({CTokenType::Token::TTYPE});
    production = new Production(productionNum, CTokenType::Token::OPT_SPECIFIERS, 0, *right);
    addProduction(*production, true);
    productionNum++;*/

    //OPT_SPECIFIERS -> SPECIFIERS
    right = new vector<CTokenType::Token>({CTokenType::Token::SPECIFIERS});
    production = new Production(productionNum, CTokenType::Token::OPT_SPECIFIERS, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //SPECIFIERS -> TYPE_OR_CLASS
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE_OR_CLASS});
    production = new Production(productionNum, CTokenType::Token::SPECIFIERS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //SPECIFIERS -> SPECIFIERS TYPE_OR_CLASS
    right = new vector<CTokenType::Token>({CTokenType::Token::SPECIFIERS, CTokenType::Token::TYPE_OR_CLASS});
    production = new Production(productionNum, CTokenType::Token::SPECIFIERS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TYPE_OR_CLASS -> TYPE_SPECIFIER
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE_SPECIFIER});
    production = new Production(productionNum, CTokenType::Token::TYPE_OR_CLASS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TYPE_SPECIFIER ->  TYPE
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE});
    production = new Production(productionNum, CTokenType::Token::TYPE_SPECIFIER, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //NEW_NAME -> NAME
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME});
    production = new Production(productionNum, CTokenType::Token::NEW_NAME, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL ->  NEW_NAME
    right = new vector<CTokenType::Token>({CTokenType::Token::NEW_NAME});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL ->START VAR_DECL
    right = new vector<CTokenType::Token>({CTokenType::Token::STAR, CTokenType::Token::VAR_DECL});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    addTerminalToSymbolMapAndArray();
}

void GrammarInitializer::addProduction(Production &production, bool nullable) {
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
    addSymbolMapAndArray(production, nullable);
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

void GrammarInitializer::addTerminalToSymbolMapAndArray() {
    for (int i = CTokenType::FIRST_TERMINAL_INDEX; i <= CTokenType::LAST_TERMINAL_INDEX; i++) {
        Symbols *symbols = new Symbols((CTokenType::Token)i, false, NULL);
        symbolArray.push_back(symbols);
        symbolMap[(CTokenType::Token)i] = symbols;
    }
}