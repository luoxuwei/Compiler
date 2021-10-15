//
// Created by 罗旭维 on 2021/10/12.
//

#include "LRStateTableParser.h"
#include "GrammarInitializer.h"
#include "Declarator.h"

LRStateTableParser::LRStateTableParser(CLexer *l) : lexer(l) {
    statusStack.push(0);//stmt
    valueStack.push_back(NULL);
    lexer->advance();
    lexerInput = CTokenType::EXT_DEF_LIST;
    lrStateTable = GrammarStateManager::getInstance()->getLRStateTable();
    showCurrentStateInfo(0);
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

void LRStateTableParser::showCurrentStateInfo(int stateNum) {
    printf("current input is :%s\n", CTokenType::getSymbolStr(lexerInput));
    printf("current state is:\n");
    GrammarStateManager::getInstance()->getGrammarState(stateNum)->print();
}

void LRStateTableParser::parse() {

    while (true) {
        int action = getAction(statusStack.top(), lexerInput);
        if (INT32_MAX == action) {
            //解析出错
            printf("\nThe input is denied\n");
            return;
        }

        if (action > 0) {
//            showCurrentStateInfo(action);
            //shift 操作
            statusStack.push(action);
            text = lexer->lookAheadText();

            parseStack.push(lexerInput);

            if (CTokenType::isTerminal(lexerInput)) {
                printf("Shift for input: %s\n", CTokenType::getSymbolStr(lexerInput));
                lexer->advance();
                lexerInput = lexer->token();
                valueStack.push_back(NULL);
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

void LRStateTableParser::takeActionForReduce(int productNum) {
    switch(productNum) {
        Specifier *last, *dst;
        Symbol *currentSym, *lastSym, *symbol;
        TypeLink *specifier;
        case GrammarInitializer::TYPE_TO_TYPE_SPECIFIER:
            attributeForParentNode = typeSystem.newType(text);
            break;
        case GrammarInitializer::CLASS_TO_TypeOrClass:
            attributeForParentNode = typeSystem.newClass(text);
            break;
        case GrammarInitializer::SPECIFIERS_TypeOrClass_TO_SPECIFIERS:
            attributeForParentNode = valueStack.back();
            last = (Specifier *)((TypeLink *)valueStack.at(valueStack.size() - 2))->getTypeObject();
            dst = (Specifier *)((TypeLink *)attributeForParentNode)->getTypeObject();
            typeSystem.specifierCpy(dst, last);
            break;
        case GrammarInitializer::NAME_TO_NewName:
            attributeForParentNode = typeSystem.newSymbol(text, nestingLevel);
            break;
        case GrammarInitializer::START_VarDecl_TO_VarDecl:
            typeSystem.addDeclarator((Symbol *)attributeForParentNode, Declarator::POINTER);
            break;
        case GrammarInitializer::ExtDeclList_COMMA_ExtDecl_TO_ExtDeclList:
            currentSym = (Symbol *)attributeForParentNode;
            lastSym = (Symbol *)valueStack.at(valueStack.size() - 3);
            currentSym->setNextSymbol(lastSym);
            break;
        case GrammarInitializer::OptSpecifier_ExtDeclList_Semi_TO_ExtDef:
            symbol = (Symbol *)attributeForParentNode;
            specifier = (TypeLink *)(valueStack.at(valueStack.size() - 3));
            typeSystem.addSpecifierToDeclaration(specifier, symbol);
            typeSystem.addSymbolsToTable(symbol);
            break;

    }
}

int LRStateTableParser::getAction(int currentState, CTokenType::Token currentInput) {
    auto iter = lrStateTable->find(currentState);
    if (iter == lrStateTable->end()) {
        return INT32_MAX;
    }
    return iter->second[currentInput];
}

