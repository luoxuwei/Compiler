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

GrammarInitializer::GrammarInitializer() {
}

void GrammarInitializer::initProductions() {
    initVariableDecalationProductions();
    initFunctionProductions();
    initStructureProductions();
    addTerminalToSymbolMapAndArray();
}

GrammarInitializer::ProductionMap* GrammarInitializer::getProductionMap() {
    return &productionMap;
}

void GrammarInitializer::initVariableDecalationProductions() {
    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;
     //long int *x, y;
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
    productionNum = 0;
    //PROGRAM -> EXT_DEF_LIST (0)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DEF_LIST});//getProductionRight(productionRight);
    production = new Production(productionNum, CTokenType::Token::PROGRAM, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //EXT_DEF_LIST -> EXT_DEF_LIST EXT_DEF (1)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DEF_LIST, CTokenType::Token::EXT_DEF});
    production = new Production(productionNum, CTokenType::EXT_DEF_LIST, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //EXT_DEF -> OPT_SPECIFIERS EXT_DECL_LIST  SEMI (2)
    right = new vector<CTokenType::Token>({ CTokenType::Token::OPT_SPECIFIERS, CTokenType::Token::EXT_DECL_LIST, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::EXT_DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXT_DEF -> OPT_SPECIFIERS  SEMI (3)
    right = new vector<CTokenType::Token>({ CTokenType::Token::OPT_SPECIFIERS, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::EXT_DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXT_DECL_LIST ->   EXT_DECL (4)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DECL});
    production = new Production(productionNum, CTokenType::Token::EXT_DECL_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    ///EXT_DECL_LIST ->EXT_DECL_LIST COMMA EXT_DECL (5)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXT_DECL_LIST, CTokenType::Token::COMMA, CTokenType::Token::EXT_DECL});
    production = new Production(productionNum, CTokenType::Token::EXT_DECL_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXT_DECL -> VAR_DECL (6)
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

    //OPT_SPECIFIERS -> SPECIFIERS (7)
    right = new vector<CTokenType::Token>({CTokenType::Token::SPECIFIERS});
    production = new Production(productionNum, CTokenType::Token::OPT_SPECIFIERS, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //SPECIFIERS -> TYPE_OR_CLASS (8)
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE_OR_CLASS});
    production = new Production(productionNum, CTokenType::Token::SPECIFIERS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //SPECIFIERS -> SPECIFIERS TYPE_OR_CLASS (9)
    right = new vector<CTokenType::Token>({CTokenType::Token::SPECIFIERS, CTokenType::Token::TYPE_OR_CLASS});
    production = new Production(productionNum, CTokenType::Token::SPECIFIERS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TYPE_OR_CLASS -> TYPE_SPECIFIER (10)
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE_SPECIFIER});
    production = new Production(productionNum, CTokenType::Token::TYPE_OR_CLASS, 0, *right);
    addProduction(*production, false);
    productionNum++;

/*    //TYPE_OR_CLASS -> CLASS   !newly add!
    right = new vector<CTokenType::Token>({CTokenType::Token::CLASS});
    production = new Production(productionNum,CTokenType::Token::TYPE_OR_CLASS, 0, *right);
    productionNum++;
    addProduction(*production, false);*/

    //TYPE_SPECIFIER ->  TYPE (11)
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE});
    production = new Production(productionNum, CTokenType::Token::TYPE_SPECIFIER, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //NEW_NAME -> NAME (12)
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME});
    production = new Production(productionNum, CTokenType::Token::NEW_NAME, 0, *right);
    addProduction(*production, false);
    productionNum++;

    /*
    //NAME_NT -> NAME
    right = getProductionRight(new int[]{CTokenType.NAME.ordinal()});
    production = new Production(productionNum,CTokenType.NAME_NT.ordinal(), 0, right);
    productionNum++;
    addProduction(production, false);
    */

    //VAR_DECL ->  NEW_NAME (13)
    right = new vector<CTokenType::Token>({CTokenType::Token::NEW_NAME});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;
    /*
    //VAR_DECL -> VAR_DECL LP RP
    right = getProductionRight(new int[]{CTokenType.VAR_DECL.ordinal(), CTokenType.LP.ordinal(), CTokenType.RP.ordinal()});
    production = new Production(productionNum,CTokenType.VAR_DECL.ordinal(), 0, right);
    productionNum++;
    addProduction(production, false);

    //VAR_DECL ->VAR_DECL LB RB
    right = getProductionRight(new int[]{CTokenType.VAR_DECL.ordinal(), CTokenType.LB.ordinal(), CTokenType.RB.ordinal()});
    production = new Production(productionNum,CTokenType.VAR_DECL.ordinal(), 0, right);
    productionNum++;
    addProduction(production, false);
    */
    //VAR_DECL ->START VAR_DECL (14)
    right = new vector<CTokenType::Token>({CTokenType::Token::STAR, CTokenType::Token::VAR_DECL});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    /*
    //VAR_DECL ->LP VAR_DECL RP
    right = getProductionRight(new int[]{CTokenType.LP.ordinal(), CTokenType.VAR_DECL.ordinal(), CTokenType.RP.ordinal()});
    production = new Production(productionNum,CTokenType.VAR_DECL.ordinal(), 0, right);
    productionNum++;
    addProduction(production, false);
    */

}

void GrammarInitializer::initFunctionProductions() {
    //void f(int x, short y, long z);
    /*production num begin with 15
    *
    * EXT_DEF -> OPT_SPECIFIERS FUNCT_DECL SEMI
     * 对应一个完整的函数声明语句 void f(int x, int y); OPT_SPECIFIERS 对应的是void，FUNCT_DECL 对应的是f(int x, int y)， SEMI对应的是分号
    *
     * FUNCT_DECL 对应两种情况, 有参数的情况，NEW_NAME 对应的是f，LP对应(, VAR_LIST对应的是变量列表, RP对应)
     * 无参数的情况，对应的是 函数名 NEW_NAME, ( LP, ) RP
    * FUNCT_DECL -> NEW_NAME LP VAR_LIST RP
    *              | NEW_NAME LP RP
     *
     * 变量列表对应 一个变量声明PARAM_DECLARATION， 或者是一个变量列表VAR_LIST， 后面跟一个逗号 COMMA(,) 再跟一个变量声明
    * VAR_LIST ->  PARAM_DECLARATION
    *              | VAR_LIST COMMA PARAM_DECLARATION
     *
     * 变量声明可以解析为 前面一个类型 TYPE_NT， 接下来是一个变量所对应的一个VAR_DECL，用来表示一个变量，VAR_DECL 对应 int x 中的x
    * PARAM_DECLARATION -> TYPE_NT VAR_DECL
    *
     * TYPE_NT -> TYPE_SPECIFIER
    *            | TYPE TYPE_SPECIFIER
    */
    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;
    //EXT_DEF -> OPT_SPECIFIERS FUNCT_DECL
    right = new vector<CTokenType::Token>({CTokenType::Token::OPT_SPECIFIERS, CTokenType::Token::FUNCT_DECL, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::EXT_DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //FUNCT_DECL -> NEW_NAME LP VAR_LIST RP (16)
    right = new vector<CTokenType::Token>({CTokenType::Token::NEW_NAME, CTokenType::Token::LP, CTokenType::Token::VAR_LIST, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::FUNCT_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //FUNCT_DECL ->  NEW_NAME LP RP (17)
    right = new vector<CTokenType::Token>({CTokenType::Token::NEW_NAME, CTokenType::Token::LP, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::FUNCT_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_LIST ->  PARAM_DECLARATION
    right = new vector<CTokenType::Token>({CTokenType::Token::PARAM_DECLARATION});
    production = new Production(productionNum, CTokenType::Token::VAR_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_LIST -> VAR_LIST COMMA PARAM_DECLARATION (19)
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_LIST, CTokenType::Token::COMMA, CTokenType::Token::PARAM_DECLARATION});
    production = new Production(productionNum, CTokenType::Token::VAR_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //PARAM_DECLARATION -> TYPE_NT VAR_DECL (20)
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE_NT, CTokenType::Token::VAR_DECL});
    production = new Production(productionNum, CTokenType::Token::PARAM_DECLARATION, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TYPE_NT -> TYPE_SPECIFIER
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE_SPECIFIER});
    production = new Production(productionNum, CTokenType::Token::TYPE_NT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TYPE_NT -> TYPE TYPE_SPECIFIER
    right = new vector<CTokenType::Token>({CTokenType::Token::TYPE, CTokenType::Token::TYPE_SPECIFIER});
    production = new Production(productionNum, CTokenType::Token::TYPE_NT, 0, *right);
    addProduction(*production, false);
    productionNum++;
}

void GrammarInitializer::initStructureProductions() {
/*    struct tag {
        int x;
        long y;
        char z;
        struct tag* p;
    }name;*/
    /* production number begin from 23
     *
     * STRUCT_SPECIFIER是专门用来定义结构体的
     * TYPE_SPECIFIER -> STRUCT_SPECIFIER
     *
     * STRUCT 是struct关键字对应的标签， OPT_TAG 是struct关键字后面的名字，LC左大括号，DEF_LIST括号里的变量定义，RC右括号，最后的变量名跟前面变量声明的语法一样，这里没有写出来，这一整个表达式就描述了struct的定义。
     * STTUCT_SPECIFIER -> STRUCT OPT_TAG LC DEF_LIST RC
     *                     | STRUCT TAG
     *
     * TAG是struct关键字后面的名字，就是这个额结构体类型的名字，这个额名字是可以没有的，所以另外加了一个OPT_TAG，这个OPT_TAG可以是TAG，也可以是null，所以这个表达式是nullable的
     * OPT_TAG -> TAG
     *
     * TAG -> NAME
     *
     *
     *一个 int x;对应一个DEF, DEF_LIST是对DEF的重复，是DEF反复连接所形成的
     * DEF_LIST ->  DEF
     * DEF_LIST ->  DEF_LIST DEF
     *
     *
     *
     *
     *
     * DEF -> SPECIFIERS  DECL_LIST SEMI
     *        | SPECIFIERS SEMI
     *
     *
     * DECL_LIST -> DECL
     *             | DECL_LIST COMMA DECL
     *
     * DECL -> VAR_DECL
     *
     * VAR_DECL 对应一个变量声明（int x;里的x)
     * VAR_DECL -> NEW_NAME
     *             | VAR_DECL LP RP
     *             | VAR_DECL LP VAR_LIST RP
     *             | LP VAR_DECL RP
     *             | START VAR_DECL
     */
    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;
    //
    //TYPE_SPECIFIER -> STRUCT_SPECIFIER  (23)
    right = new vector<CTokenType::Token>({CTokenType::Token::STRUCT_SPECIFIER});
    production = new Production(productionNum, CTokenType::Token::TYPE_SPECIFIER, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STTUCT_SPECIFIER -> STRUCT OPT_TAG LC DEF_LIST RC (24)
    right = new vector<CTokenType::Token>({CTokenType::Token::STRUCT, CTokenType::Token::OPT_TAG, CTokenType::Token::LC, CTokenType::Token::DEF_LIST, CTokenType::Token::RC});
    production = new Production(productionNum, CTokenType::Token::STRUCT_SPECIFIER, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STRUCT_SPECIFIER -> STRUCT TAG (25)
    right = new vector<CTokenType::Token>({CTokenType::Token::STRUCT, CTokenType::Token::TAG});
    production = new Production(productionNum, CTokenType::Token::STRUCT_SPECIFIER, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //OPT_TAG -> TAG (26)
    right = new vector<CTokenType::Token>({CTokenType::Token::TAG});
    production = new Production(productionNum, CTokenType::Token::OPT_TAG, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //TAG -> NAME (27)
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME});
    production = new Production(productionNum, CTokenType::Token::TAG, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DEF_LIST ->  DEF (28)
    right = new vector<CTokenType::Token>({CTokenType::Token::DEF});
    production = new Production(productionNum, CTokenType::Token::DEF_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DEF_LIST -> DEF_LIST DEF (29)
    right = new vector<CTokenType::Token>({CTokenType::Token::DEF_LIST, CTokenType::Token::DEF});
    production = new Production(productionNum, CTokenType::Token::DEF_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DEF -> SPECIFIERS DECL_LIST SEMI (30)
    right = new vector<CTokenType::Token>({CTokenType::Token::SPECIFIERS, CTokenType::Token::DECL_LIST, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DEF -> SPECIFIERS SEMI (31)
    right = new vector<CTokenType::Token>({CTokenType::Token::SPECIFIERS, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DECL_LIST -> DECL (32)
    right = new vector<CTokenType::Token>({CTokenType::Token::DECL});
    production = new Production(productionNum, CTokenType::Token::DECL_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DECL_LIST -> DECL_LIST COMMA DECL (33)
    right = new vector<CTokenType::Token>({CTokenType::Token::DECL_LIST, CTokenType::Token::COMMA, CTokenType::Token::DECL});
    production = new Production(productionNum, CTokenType::Token::DECL_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DECL -> VAR_DECL (34)
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL});
    production = new Production(productionNum, CTokenType::Token::DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> NEW_NAME (35)
    right = new vector<CTokenType::Token>({CTokenType::Token::NEW_NAME});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> VAR_DECL LP RP (36)
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL, CTokenType::Token::LP, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> VAR_DECL LP VAR_LIS RP (37)
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL, CTokenType::Token::LP, CTokenType::Token::VAR_LIST, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> LP VAR_DECL RP (38)
    right = new vector<CTokenType::Token>({CTokenType::Token::LP, CTokenType::Token::VAR_DECL, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> STAR VAR_DECL (39)
    right = new vector<CTokenType::Token>({CTokenType::Token::STAR, CTokenType::Token::VAR_DECL});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;
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