//
// Created by 罗旭维 on 2021/10/12.
//

#include "LRStateTableParser.h"
#include "GrammarInitializer.h"
#include "Declarator.h"
#include "CodeTreeBuilder.h"

LRStateTableParser *LRStateTableParser::instance = NULL;

LRStateTableParser * LRStateTableParser::getInstance() {
    if (instance == NULL) {
        instance = new LRStateTableParser();
    }
    return instance;
}

LRStateTableParser::LRStateTableParser() {
    typeSystem = TypeSystem::getInstance();
    lrStateTable = GrammarStateManager::getInstance()->getLRStateTable();
}

const char * LRStateTableParser::new_name() {
    if (curName >= 8) {
        printf("\nexpression too complicated\n");
        throw 0;
    }
    return names[curName++];
}

void LRStateTableParser::free_name(const char *s) {
    names[--curName] = s;
}

string * LRStateTableParser::getRelOpereatorText() {
    return &relOperatorText;
}

void LRStateTableParser::showCurrentStateInfo(int stateNum) {
    printf("current input is :%s\n", CTokenType::getSymbolStr(lexerInput));
    printf("current state is:\n");
    GrammarStateManager::getInstance()->getGrammarState(stateNum)->print();
}

void LRStateTableParser::parse(CLexer *l) {
    lexer = l;
    statusStack.push(0);//stmt
    valueStack.push_back(NULL);
    lexer->advance();
    lexerInput = CTokenType::EXT_DEF_LIST;
    showCurrentStateInfo(0);

    while (true) {
        int action = getAction(statusStack.top(), lexerInput);
        if (INT32_MAX == action) {
            //解析出错
            printf("\nShift for input: %s\n", CTokenType::getSymbolStr(lexerInput));
            printf("\nThe input is denied\n");
            return;
        }

        if (action > 0) {
//            showCurrentStateInfo(action);
            //shift 操作
            statusStack.push(action);
            text = lexer->lookAheadText();
            if (lexerInput == CTokenType::Token::RELOP) {
                relOperatorText = text;
            }

            parseStack.push(lexerInput);

            if (CTokenType::isTerminal(lexerInput)) {
                printf("Shift for input: %s\n", CTokenType::getSymbolStr(lexerInput));
                void *obj = takeActionForShift(lexerInput);
                lexer->advance();
                lexerInput = lexer->token();
                valueStack.push_back(obj);
            } else {
                //输入是非终结符，不往下读取，恢复到上个输入。
                lexerInput = lexer->token();
            }
        } else {
            if (action == 0) {
                printf("\nThe input can be accepted\n");
                return;
            }
            int reduceProduction = -action;
            Production *production = ProductionManager::getInstance()->getProductionByIndex(reduceProduction);
            printf("reduce by production: \n");
            production->print();

            takeActionForReduce(reduceProduction);

            int rightSize = production->getRight().size();
            while (rightSize > 0) {
                statusStack.pop();
                parseStack.pop();
                valueStack.pop_back();
                rightSize--;
            }
            lexerInput = production->getLeft();
            parseStack.push(lexerInput);
            valueStack.push_back(attributeForParentNode);
        }
    }

}

void *LRStateTableParser::takeActionForShift(CTokenType::Token token) {
    if (token == CTokenType::Token::LP || token == CTokenType::Token::LC) {
        nestingLevel++;
    }

    if (token == CTokenType::Token::RP || token == CTokenType::Token::RC) {
        nestingLevel--;
    }
    return NULL;
}

void LRStateTableParser::takeActionForReduce(int productNum) {
    switch(productNum) {
        Specifier *last, *dst, *sp;
        Symbol *currentSym, *lastSym, *symbol, *argList, *defList;
        TypeLink *specifier;
        StructDefine *structObj;
        Declarator *declarator;
        int arrayNum;
        case GrammarInitializer::TYPE_TO_TYPE_SPECIFIER:
            attributeForParentNode = typeSystem->newType(text);
            break;
        case GrammarInitializer::EnumSpecifier_TO_TypeSpecifier:
            attributeForParentNode = typeSystem->newType("int");
            break;
/*        case GrammarInitializer::CLASS_TO_TypeOrClass:
            attributeForParentNode = typeSystem.newClass(text);
            break;*/
        case GrammarInitializer::StructSpecifier_TO_TypeSpecifier:
            attributeForParentNode = typeSystem->newType(text);
            specifier = (TypeLink *) attributeForParentNode;
            sp = (Specifier *) specifier->getTypeObject();
            sp->setType(Specifier::STRUCTURE);
            structObj = (StructDefine *) valueStack.at(valueStack.size() - 1);
            sp->setStructObj(structObj);
            break;
        case GrammarInitializer::SPECIFIERS_TypeOrClass_TO_SPECIFIERS:
            attributeForParentNode = valueStack.back();
            last = (Specifier *)((TypeLink *)valueStack.at(valueStack.size() - 2))->getTypeObject();
            dst = (Specifier *)((TypeLink *)attributeForParentNode)->getTypeObject();
            typeSystem->specifierCpy(dst, last);
            break;
        case GrammarInitializer::NAME_TO_NewName://声明变量或函数
        case GrammarInitializer::Name_TO_NameNT:
            attributeForParentNode = typeSystem->newSymbol(text, nestingLevel);
            break;
        case GrammarInitializer::START_VarDecl_TO_VarDecl:
        case GrammarInitializer::Start_VarDecl_TO_VarDecl:
            typeSystem->addDeclarator((Symbol *)attributeForParentNode, Declarator::POINTER);
            break;
        case GrammarInitializer::VarDecl_LB_ConstExpr_RB_TO_VarDecl:
            //数组定义
            declarator = typeSystem->addDeclarator((Symbol *) valueStack.at(valueStack.size() - 4), Declarator::ARRAY);
            arrayNum = (long) attributeForParentNode;
            declarator->setElementNum(arrayNum);
            attributeForParentNode = valueStack.at(valueStack.size() - 4);
            break;
        case GrammarInitializer::Name_TO_Unary://调用时直接从符号表中查找，符号已在声明时创建
            attributeForParentNode = typeSystem->getSymbolByText(text, nestingLevel, *symbolScope);
            break;
        case GrammarInitializer::ExtDeclList_COMMA_ExtDecl_TO_ExtDeclList:
        case GrammarInitializer::VarList_COMMA_ParamDeclaration_TO_VarList:
        case GrammarInitializer::DeclList_Comma_Decl_TO_DeclList:
        case GrammarInitializer::DefList_Def_TO_DefList:
            currentSym = (Symbol *)attributeForParentNode;
            if (productNum == GrammarInitializer::DefList_Def_TO_DefList) {
                lastSym = (Symbol *)valueStack.at(valueStack.size() - 2);
            } else {
                lastSym = (Symbol *)valueStack.at(valueStack.size() - 3);
            }
            currentSym->setNextSymbol(lastSym);
            break;
        case GrammarInitializer::OptSpecifier_ExtDeclList_Semi_TO_ExtDef://这几个都是完整的变量定义表达式（包括函数参数定义)
        case GrammarInitializer::TypeNT_VarDecl_TO_ParamDeclaration:
        case GrammarInitializer::Specifiers_DeclList_Semi_TO_Def:
            symbol = (Symbol *)attributeForParentNode;
            specifier = (TypeLink *)(valueStack.at(valueStack.size() - 3));
            typeSystem->addSpecifierToDeclaration(specifier, symbol);
            typeSystem->addSymbolsToTable(symbol, symbolScope);//在这里还不知道变量是不是函数参数，先将变量作用域设置成global，到解析到函数申明（整个函数头)时知道是函数参数后改为函数名
            break;
        case GrammarInitializer::VarDecl_Equal_Initializer_TO_Decl:
            //如果这里不把attributeForParentNode设置成对应的symbol对象
            //那么上面执行 Symbol symbol = (Symbol)attributeForParentNode; 会出错
            attributeForParentNode = valueStack.at(valueStack.size() - 2);
            break;
        case GrammarInitializer::NewName_LP_VarList_RP_TO_FunctDecl:
            setFunctionSymbol(true);
            argList = (Symbol *) valueStack.at(valueStack.size() -2);
            ((Symbol *) attributeForParentNode)->args = argList;
            typeSystem->addSymbolsToTable((Symbol *) attributeForParentNode, symbolScope);
            //参数声明的解析在OptSpecifier_ExtDeclList_Semi_TO_ExtDef中，解析时先设置层global,当解析到这里(函数头)，知道是参数就将作用域改为函数名
            //遇到函数定义，变量的scope名称要改为函数名,并把函数参数的scope改为函数名
            //出了函数定义后要改回global，怎么知道出了函数定义？语法推导表达式OptSpecifiers_FunctDecl_CompoundStmt_TO_ExtDef 表示完整的函数定义，解析到这个表达式（对这个表达式进行reduce)表示函数定义解析完毕。
            symbolScope = ((Symbol *) attributeForParentNode)->getName();
            symbol = argList;
            while (symbol != NULL) {
                symbol->addScope(symbolScope);
                symbol = symbol->getNextSymbol();
            }
            break;
        case GrammarInitializer::OptSpecifiers_FunctDecl_CompoundStmt_TO_ExtDef:
            symbol = (Symbol *) valueStack.at(valueStack.size() - 2);
            specifier = (TypeLink *) valueStack.at(valueStack.size() - 3);
            typeSystem->addSpecifierToDeclaration(specifier, symbol);

            //解析到这里表示函数定义结束，函数定义结束后，接下来的变量作用范围应该改为global
            symbolScope = &GLOBAL_SCOPE;
            break;
        case GrammarInitializer::NewName_LP_RP_TO_FunctDecl:
            setFunctionSymbol(false);
            typeSystem->addSymbolsToTable((Symbol *) attributeForParentNode, symbolScope);
            //遇到函数定义，变量的scope名称要改为函数名
            symbolScope = ((Symbol *) attributeForParentNode)->getName();
            break;
        case GrammarInitializer::Name_To_Tag:
            attributeForParentNode = typeSystem->getStructObjFromTable(text);
            if (attributeForParentNode == NULL) {
                attributeForParentNode = new StructDefine(text, nestingLevel, NULL);
                typeSystem->addStructToTable((StructDefine *) attributeForParentNode);
            }
            break;
        case GrammarInitializer::Struct_OptTag_LC_DefList_RC_TO_StructSpecifier:
            defList = (Symbol *) valueStack.at(valueStack.size() - 2);
            structObj = (StructDefine *) valueStack.at(valueStack.size() - 4);
            structObj->setFields(defList);
            attributeForParentNode = structObj;
            break;
        case GrammarInitializer::Enum_TO_EnumNT:
            enumValue = 0;
            break;
        case GrammarInitializer::NameNT_TO_Emurator:
            doEnum();
            break;
        case GrammarInitializer::Name_Eequal_ConstExpr_TO_Enuerator:
            enumValue = (long) valueStack.at(valueStack.size() - 1);
            attributeForParentNode = valueStack.at(valueStack.size() - 3);
            doEnum();
            break;
        case GrammarInitializer::Number_TO_ConstExpr:
        case GrammarInitializer::Number_TO_Unary:
            attributeForParentNode = (void *) stoi(text);
            break;
    }

    CodeTreeBuilder::getInstance()->buildCodeTree(productNum, text);
}

void LRStateTableParser::doEnum() {
    Symbol *symbol = (Symbol *) attributeForParentNode;
    if (convSymToIntConst(symbol, enumValue)) {
        typeSystem->addSymbolsToTable(symbol, symbolScope);
        enumValue++;
    } else {
        printf("enum symbol redefinition: %s\n", symbol->name.c_str());
    }

}

bool LRStateTableParser::convSymToIntConst(Symbol *symbol, int val) {
    if (symbol->getTypeHead() != NULL) {
        return false;
    }

    TypeLink *typeLink = typeSystem->newType("int");
    Specifier *specifier = (Specifier *) typeLink->getTypeObject();
    specifier->setConstantVal(val);
    specifier->setType(Specifier::CONSTANT);
    symbol->addSpecifier(typeLink);
    return true;
}

void LRStateTableParser::setFunctionSymbol(bool hasArgs) {
    Symbol *funcSymbol;
    if (hasArgs) {
        funcSymbol = (Symbol *) valueStack.at(valueStack.size() - 4);
    } else {
        funcSymbol = (Symbol *) valueStack.at(valueStack.size() - 3);
    }
    typeSystem->addDeclarator(funcSymbol, Declarator::FUNCTION);
    attributeForParentNode = funcSymbol;
}

int LRStateTableParser::getAction(int currentState, CTokenType::Token currentInput) {
    auto iter = lrStateTable->find(currentState);
    if (iter == lrStateTable->end()) {
        return INT32_MAX;
    }
    return iter->second[currentInput];
}

deque<void *> * LRStateTableParser::getValueStack() {
    return &valueStack;
}

