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
    initEmunProductions();
    initFunctionDefinition();
    initFunctionDefinition2();
    initFunctionDefinitionWithIfElse();
    initFunctionDefinitionWithSwitchCase();
    initFunctionDefinitionWithLoop();
    initComputingOperation();
    initRemaindingProduction();
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
//    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> VAR_DECL LP VAR_LIS RP (37)
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL, CTokenType::Token::LP, CTokenType::Token::VAR_LIST, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::VAR_DECL, 0, *right);
//    addProduction(*production, false);
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

void GrammarInitializer::initEmunProductions() {
    //enum tag {a, b, c=5, d} x;
    /*
     * begin from production number 40
     *
     */
    //ENUM_SPECIFIER对应整个enum定义（enum tag {...}）,ENUM_NT对应关键字enum，NAME_NT 对应tag,OPT_ENUM_LIST对应的是后面{}这一段
    //ENUM_SPECIFIER -> ENUM_NT NAME_NT OPT_ENUM_LIST(40)
    //ENUM_NT -> ENUM(41)
    //ENUMERATOR_LIST -> ENUMERATOR(42)
    //EMERATOR_LIST -> ENUMERATOR_LIST COMMA ENUMERATOR(43)
    //ENUMERATOR -> NAME_NT(44)
    //NAME_NT -> NAME(45)
    //给一个变量赋初值对应（ c=5 ) EQUAL对应等号，后面的常量对应 CONST_EXPR
    //ENUMERATOR -> NAME_NT EQUAL CONST_EXPR(46)
    //把CONST_EXPR做了简化处理，变成一个NUMBER来处理，也就是看到一个数字常量的时候会自动推导成一个CONST_EXPR, 实际上CONST_EXPR的推导还是比较复杂的，只是这里做了简化
    //CONST_EXPR -> NUMBER (47)
    //OPT_ENUM_LIST -> LC ENUMERATOR_LIST RC (48)
    //TYPE_SPECIFIER -> ENUM_SPECIFIER (49)

    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //ENUM_SPECIFIER -> ENUM_NT NAME_NT OPT_ENUM_LIST(40)
    right = new vector<CTokenType::Token>({CTokenType::Token::ENUM_NT, CTokenType::Token::NAME_NT, CTokenType::Token::OPT_ENUM_LIST});
    production = new Production(productionNum, CTokenType::Token::ENUM_SPECIFIER, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //ENUM_NT -> ENUM(41)
    right = new vector<CTokenType::Token>({CTokenType::Token::ENUM});
    production = new Production(productionNum, CTokenType::Token::ENUM_NT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //ENUMERATOR_LIST -> ENUMERATOR(42)
    right = new vector<CTokenType::Token>({CTokenType::Token::ENUMERATOR});
    production = new Production(productionNum, CTokenType::Token::ENUMERATOR_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EMERATOR_LIST -> ENUMERATOR_LIST COMMA ENUMERATOR(43)
    right = new vector<CTokenType::Token>({CTokenType::Token::ENUMERATOR_LIST, CTokenType::Token::COMMA, CTokenType::Token::ENUMERATOR});
    production = new Production(productionNum, CTokenType::Token::ENUMERATOR_LIST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //ENUMERATOR -> NAME_NT(44)
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME_NT});
    production = new Production(productionNum, CTokenType::Token::ENUMERATOR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //NAME_NT -> NAME(45)
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME});
    production = new Production(productionNum, CTokenType::Token::NAME_NT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //ENUMERATOR -> NAME_NT EQUAL CONST_EXPR(46)
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME_NT, CTokenType::Token::EQUAL, CTokenType::Token::CONST_EXPR});
    production = new Production(productionNum, CTokenType::Token::ENUMERATOR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //CONST_EXPR -> NUMBER (47)
    right = new vector<CTokenType::Token>({CTokenType::Token::NUMBER});
    production = new Production(productionNum, CTokenType::Token::CONST_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //OPT_ENUM_LIST -> LC ENUMERATOR_LIST RC (48)
    right = new vector<CTokenType::Token>({CTokenType::Token::LC, CTokenType::Token::ENUMERATOR_LIST, CTokenType::Token::RC});
    production = new Production(productionNum, CTokenType::Token::OPT_ENUM_LIST, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //TYPE_SPECIFIER -> ENUM_SPECIFIER (49)
    right = new vector<CTokenType::Token>({CTokenType::Token::ENUM_SPECIFIER});
    production = new Production(productionNum, CTokenType::Token::TYPE_SPECIFIER, 0, *right);
    addProduction(*production, true);
    productionNum++;


}

void GrammarInitializer::initFunctionDefinition() {

    /*
     * begin production number 50
     */
/*    void f() {
        int a;
        int b;
        a = 1;
        b = 2;
        int c;
        c = a > b ? a : b;

        return c;
    }*/
    //OPT_SPECIFIERS对应void，FUNCT_DECL对应f(),COMPOUND_STMT覆盖了后面大括号这一段
    //EXT_DEF -> OPT_SPECIFIERS FUNCT_DECL COMPOUND_STMT(50)

    //LC是右括号，LOCAL_DEFS是开始的a,b两个变量定义，STMT_LIST后面那一段
    //COMPOUND_STMT-> LC LOCAL_DEFS STMT_LIST RC(51)
    //本地变量定义
    //LOCAL_DEFS -> DEF_LIST(52)
    //EXPR表示表达式
    //EXPR -> NO_COMMA_EXPR(53)

    //中间的EQUAL是等号，整个表示对应 a = 1;这种类型的表达式，a和1都对应NO_COMMA_EXPR，中间的等号是EQUAL
    //NO_COMMA_EXPR -> NO_COMMA_EXPR EQUAL NO_COMMA_EXPR(54)
    //QUEST对应？号，NO_COMMA_EXPR-->BINARY-->BINARY RELOP BINARY, 所以整个表示a > b ? a : b这种表达式
    //NO_COMMA_EXPR -> NO_COMMA_EXPR QUEST  NO_COMMA_EXPR COLON NO_COMMA_EXPR(55)

    //NO_COMMA_EXPR -> BINARY(56)

    //BINARY -> UNARY (57)

    //UNARY -> NUMBER (58)

    //UNARY -> NAME (59)

    //UNARY -> STRING(60)

    //STMT_LIST -> STMT_LIST STATEMENT(61)

    //STMT_LIST ->  STATEMENT(62)

    //STATEMENT -> EXPR SEMI(63)

    //STATEMENT -> RETURN EXPR SEMI (64)

    //RELOP对应的是比较符号>< >= <=都对应RELOP，整个表示 a>b这种表达式
    //BINARY -> BINARY RELOP BINARY (65)

    //EQUOP是等于比较 == !=
    //BINARY -> BINARY EQUOP BINARY (66)

    //STAR 对应 *号
    //BINARY -> BINARY START BINARY (66)

    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //EXT_DEF -> OPT_SPECIFIERS FUNCT_DECL COMPOUND_STMT(50)
    right = new vector<CTokenType::Token>({CTokenType::Token::OPT_SPECIFIERS, CTokenType::Token::FUNCT_DECL, CTokenType::Token::COMPOUND_STMT});
    production = new Production(productionNum, CTokenType::Token::EXT_DEF, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //COMPOUND_STMT-> LC LOCAL_DEFS STMT_LIST RC(51)
    right = new vector<CTokenType::Token>({CTokenType::Token::LC, CTokenType::Token::LOCAL_DEFS, CTokenType::Token::STMT_LIST, CTokenType::Token::RC});
    production = new Production(productionNum, CTokenType::Token::COMPOUND_STMT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //LOCAL_DEFS -> DEF_LIST(52)
    right = new vector<CTokenType::Token>({CTokenType::Token::DEF_LIST});
    production = new Production(productionNum, CTokenType::Token::LOCAL_DEFS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //EXPR -> NO_COMMA_EXPR(53)
    right = new vector<CTokenType::Token>({CTokenType::Token::NO_COMMA_EXPR});
    production = new Production(productionNum, CTokenType::Token::EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //NO_COMMA_EXPR -> NO_COMMA_EXPR EQUAL NO_COMMA_EXPR(54)
    right = new vector<CTokenType::Token>({CTokenType::Token::NO_COMMA_EXPR, CTokenType::Token::EQUAL, CTokenType::Token::NO_COMMA_EXPR});
    production = new Production(productionNum, CTokenType::Token::NO_COMMA_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //NO_COMMA_EXPR -> NO_COMMA_EXPR QUEST  NO_COMMA_EXPR COLON NO_COMMA_EXPR(55)
    right = new vector<CTokenType::Token>({CTokenType::Token::NO_COMMA_EXPR, CTokenType::Token::QUEST, CTokenType::Token::NO_COMMA_EXPR, CTokenType::Token::COLON, CTokenType::Token::NO_COMMA_EXPR});
    production = new Production(productionNum, CTokenType::Token::NO_COMMA_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //NO_COMMA_EXPR -> BINARY(56)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::NO_COMMA_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> UNARY (57)
    right = new vector<CTokenType::Token>({CTokenType::Token::UNARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> NUMBER (58)
    right = new vector<CTokenType::Token>({CTokenType::Token::NUMBER});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> NAME (59)
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> STRING(60)
    right = new vector<CTokenType::Token>({CTokenType::Token::STRING});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STMT_LIST -> STMT_LIST STATEMENT(61)
    right = new vector<CTokenType::Token>({CTokenType::Token::STMT_LIST, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STMT_LIST, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //STMT_LIST ->  STATEMENT(62)
    right = new vector<CTokenType::Token>({CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STMT_LIST, 0, *right);
    addProduction(*production, true);
    productionNum++;

    //STATEMENT -> EXPR SEMI(63)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXPR, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> RETURN EXPR SEMI (64)
    right = new vector<CTokenType::Token>({CTokenType::Token::RETURN, CTokenType::Token::EXPR, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY RELOP BINARY (65)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::RELOP, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY EQUOP BINARY (66)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::EQUOP, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY START BINARY (67)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::STAR, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> LOCAL_DEFS(68)
    right = new vector<CTokenType::Token>({CTokenType::Token::LOCAL_DEFS});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;
}

void GrammarInitializer::initFunctionDefinition2() {
    //COMPOUND_STMT -> LC RC(69)
    //COMPOUNT_STMT -> LC STMT_LIST RC(70)
    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //COMPOUND_STMT -> LC RC(69)
    right = new vector<CTokenType::Token>({CTokenType::Token::LC, CTokenType::Token::RC});
    production = new Production(productionNum, CTokenType::Token::COMPOUND_STMT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //COMPOUNT_STMT -> LC STMT_LIST RC(70)
    right = new vector<CTokenType::Token>({CTokenType::Token::LC, CTokenType::Token::STMT_LIST, CTokenType::Token::RC});
    production = new Production(productionNum, CTokenType::Token::COMPOUND_STMT, 0, *right);
    addProduction(*production, false);
    productionNum++;
}

void GrammarInitializer::initFunctionDefinitionWithIfElse() {
/*    void f() {
        int a = 0;
        int b = 0;

        if (i < 1)
            a = 1;
        else if (i < 2)
            a = 2;
        else
            a = 3;
    }*/
/*    void f() {
        int a = 0;
        int b = 0;

        for (int i = 0; i < 10; i++) {
            a = a + 1;
        }
    }*/
    //STATEMENT -> COMPOUND_STMT (71)
    //TEST对应if后面括号里的条件判断表达式，STATEMENT是if (i<1) 后面的部分
    //IF_STATEMENT -> IF LP TEST RP STATEMENT (72)
    //IF_ELSE_STATEMENT -> IF_STATEMENT
    //IF_ELSE_STATEMENT ->IF_ELSE_STATEMENT ELSE STATEMENT
    //STATEMENT -> IF_ELSE_STATEMENT
    //TEST -> EXPR
    // int a = 0;INITIALIZER是EXPR，EXPR可以是一个表达式，数字和变量名
    //DECL -> VAR_DECL EQUAL INITIALIZER
    //
    //INITIALIZER -> EXPR

    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //STATEMENT -> COMPOUND_STMT (71)
    right = new vector<CTokenType::Token>({CTokenType::Token::COMPOUND_STMT});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //IF_STATEMENT -> IF LP TEST RP STATEMENT (72)
    right = new vector<CTokenType::Token>({CTokenType::Token::IF, CTokenType::Token::LP, CTokenType::Token::TEST, CTokenType::Token::RP, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::IF_STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //IF_ELSE_STATEMENT -> IF_STATEMENT
    right = new vector<CTokenType::Token>({CTokenType::Token::IF_STATEMENT});
    production = new Production(productionNum, CTokenType::Token::IF_ELSE_STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //IF_ELSE_STATEMENT ->IF_ELSE_STATEMENT ELSE STATEMENT
    right = new vector<CTokenType::Token>({CTokenType::Token::IF_ELSE_STATEMENT, CTokenType::Token::ELSE, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::IF_ELSE_STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> IF_ELSE_STATEMENT
    right = new vector<CTokenType::Token>({CTokenType::Token::IF_ELSE_STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TEST -> EXPR
    right = new vector<CTokenType::Token>({CTokenType::Token::EXPR});
    production = new Production(productionNum, CTokenType::Token::TEST, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //DECL -> VAR_DECL EQUAL INITIALIZER
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL, CTokenType::Token::EQUAL, CTokenType::Token::INITIALIZER});
    production = new Production(productionNum, CTokenType::Token::DECL, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //INITIALIZER -> EXPR
    right = new vector<CTokenType::Token>({CTokenType::Token::EXPR});
    production = new Production(productionNum, CTokenType::Token::INITIALIZER, 0, *right);
    addProduction(*production, false);
    productionNum++;
}

void GrammarInitializer::initFunctionDefinitionWithSwitchCase() {
    //STATEMENT -> SWITCH LP EXPR RP COMPOUND_STATEMENT (79)

    //STATEMENT -> CASE CONST_EXPR COLON(80)

    //STATEMENT -> DEFAULT COLON (81)

    //STATEMENT -> BREAK SEMI; (82)

    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //STATEMENT -> SWITCH LP EXPR RP COMPOUND_STATEMENT (79)
    right = new vector<CTokenType::Token>({CTokenType::Token::SWITCH, CTokenType::Token::LP, CTokenType::Token::EXPR, CTokenType::Token::RP, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> CASE CONST_EXPR COLON(80)
    right = new vector<CTokenType::Token>({CTokenType::Token::CASE, CTokenType::Token::CONST_EXPR, CTokenType::Token::COLON});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> DEFAULT COLON (81)
    right = new vector<CTokenType::Token>({CTokenType::Token::DEFAULT, CTokenType::Token::COLON});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> BREAK SEMI; (82)
    right = new vector<CTokenType::Token>({CTokenType::Token::BREAK, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

}

void GrammarInitializer::initFunctionDefinitionWithLoop() {
/*    void f() {
        int a = 0;
        int i = 0;
        for (i = 0; i < 10; i++) {
            a = a + 1;
        }
    }*/
/*    void f() {
        int a = 0;
        int i = 0;
        while (i < 10) {
            a = a + 1;
            i = i + 1;
        }
    }*/
/*    void f() {
        int a = 0;
        int i = 0;
        do {
            a = a + 1;
            i = i + 1;
        } while (i < 10);
    }*/
/*    while (i<10) {
        ...
    }*/
    //STATEMENT -> WHILE LP TEST RP STATEMENT (83)

    //for (int i=0; i<10; i++)
    //for (; i<10; i++)
    //OPT_EXPR 是 int i=0这一段，可以是空的，END_OPT_EXPR 是i++
    //STATEMENT -> FOR LP OPT_EXPR  TEST SEMI END_OPT_EXPR RP STATEMENT(84)

    //OPT_EXPR -> EXPR SEMI(85)

    //OPT_EXPR -> SEMI(86)

    //END_OPT_EXPR -> EXPR(87)

    //do while 循环
    //STATEMENT -> DO STATEMENT WHILE LP TEST RP SEMI(88)

    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //STATEMENT -> WHILE LP TEST RP STATEMENT (83)
    right = new vector<CTokenType::Token>({CTokenType::Token::WHILE, CTokenType::Token::LP, CTokenType::Token::TEST, CTokenType::Token::RP, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> FOR LP OPT_EXPR  TEST SEMI END_OPT_EXPR RP STATEMENT(84)
    right = new vector<CTokenType::Token>({CTokenType::Token::FOR, CTokenType::Token::LP, CTokenType::Token::OPT_EXPR, CTokenType::Token::TEST, CTokenType::Token::SEMI,
                                           CTokenType::Token::END_OPT_EXPR, CTokenType::Token::RP, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //OPT_EXPR -> EXPR SEMI(85)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXPR, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::OPT_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //OPT_EXPR -> SEMI(86)
    right = new vector<CTokenType::Token>({CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::OPT_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //END_OPT_EXPR -> EXPR(87)
    right = new vector<CTokenType::Token>({CTokenType::Token::EXPR});
    production = new Production(productionNum, CTokenType::Token::END_OPT_EXPR, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> DO STATEMENT WHILE LP TEST RP SEMI(88)
    right = new vector<CTokenType::Token>({CTokenType::Token::DO, CTokenType::Token::STATEMENT, CTokenType::Token::WHILE, CTokenType::Token::LP, CTokenType::Token::TEST,
                                           CTokenType::Token::RP, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;
}

void GrammarInitializer::initComputingOperation() {
    //BINARY -> BINARY STAR BINARY(89)

    //BINARY -> BINARY DIVOP BINARY(90)

    //BINARY -> BINARY SHIFTOP BINARY(91)

    //BINARY -> BINARY AND BINARY(92)

    //BINARY -> BINARY XOR BINARY(93)

    //BINARY -> BINARY PLUS BINARY(94)

    //BINARY -> BINARY MINUS BINARY(95)

    //UNARY -> UNARY INCOP (96)

    //UNARY -> INCOP UNARY  (97)


    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //BINARY -> BINARY STAR BINARY(89)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::STAR, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY DIVOP BINARY(90)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::DIVOP, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY SHIFTOP BINARY(91)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::SHIFTOP, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY AND BINARY(92)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::AND, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY XOR BINARY(93)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::XOR, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY PLUS BINARY(94)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::PLUS, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //BINARY -> BINARY MINUS BINARY(95)
    right = new vector<CTokenType::Token>({CTokenType::Token::BINARY, CTokenType::Token::MINUS, CTokenType::Token::BINARY});
    production = new Production(productionNum, CTokenType::Token::BINARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> UNARY INCOP  i++ (96)
    right = new vector<CTokenType::Token>({CTokenType::Token::UNARY, CTokenType::Token::INCOP});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> INCOP UNARY  ++i (97)
    right = new vector<CTokenType::Token>({CTokenType::Token::INCOP, CTokenType::Token::UNARY});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> MINUS UNARY  a = -a
    right = new vector<CTokenType::Token>({CTokenType::Token::MINUS, CTokenType::Token::UNARY});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> STAR UNARY b = *a
    right = new vector<CTokenType::Token>({CTokenType::Token::STAR, CTokenType::Token::UNARY});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> UNARY STRUCTOP NAME  a = tag->name
    right = new vector<CTokenType::Token>({CTokenType::Token::UNARY, CTokenType::Token::STRUCTOP, CTokenType::Token::UNARY});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> UNARY LB EXPR RB b = a[2];
    right = new vector<CTokenType::Token>({CTokenType::Token::UNARY, CTokenType::Token::LB, CTokenType::Token::EXPR, CTokenType::Token::RB});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> UNARY LP ARGS RP fun(a, b ,c)
    right = new vector<CTokenType::Token>({CTokenType::Token::UNARY, CTokenType::Token::LP, CTokenType::Token::ARGS, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //UNARY -> UNARY LP RP  fun()
    right = new vector<CTokenType::Token>({CTokenType::Token::UNARY, CTokenType::Token::LP, CTokenType::Token::RP});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //ARGS -> NO_COMMA_EXPR
    right = new vector<CTokenType::Token>({CTokenType::Token::NO_COMMA_EXPR});
    production = new Production(productionNum, CTokenType::Token::ARGS, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //ARGS -> NO_COMMA_EXPR COMMA ARGS
    right = new vector<CTokenType::Token>({CTokenType::Token::NO_COMMA_EXPR, CTokenType::Token::COMMA, CTokenType::Token::ARGS});
    production = new Production(productionNum, CTokenType::Token::UNARY, 0, *right);
    addProduction(*production, false);
    productionNum++;
}

void GrammarInitializer::initRemaindingProduction() {
    //STATEMENT -> TARGET COLON STATEMENT

    //STATEMENT -> GOTO TARGET SEMI

    //TARGET -> NAME

    //VAR_DECL -> VAR_DECL LB CONST_EXPR RB  a[5] (109)

    vector<CTokenType::Token> *right = NULL;
    Production *production = NULL;

    //STATEMENT -> TARGET COLON STATEMENT
    right = new vector<CTokenType::Token>({CTokenType::Token::TARGET, CTokenType::Token::COLON, CTokenType::Token::STATEMENT});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //STATEMENT -> GOTO TARGET SEMI
    right = new vector<CTokenType::Token>({CTokenType::Token::GOTO, CTokenType::Token::TARGET, CTokenType::Token::SEMI});
    production = new Production(productionNum, CTokenType::Token::STATEMENT, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //TARGET -> NAME
    right = new vector<CTokenType::Token>({CTokenType::Token::NAME});
    production = new Production(productionNum, CTokenType::Token::TARGET, 0, *right);
    addProduction(*production, false);
    productionNum++;

    //VAR_DECL -> VAR_DECL LB CONST_EXPR RB  a[5] (109)
    right = new vector<CTokenType::Token>({CTokenType::Token::VAR_DECL, CTokenType::Token::LB, CTokenType::Token::CONST_EXPR, CTokenType::Token::RB});
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