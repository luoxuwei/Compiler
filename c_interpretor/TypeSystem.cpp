//
// Created by 罗旭维 on 2021/10/15.
//

#include "TypeSystem.h"
#include "Declarator.h"
TypeSystem *TypeSystem::instance = NULL;

TypeSystem * TypeSystem::getInstance() {
    if (instance == NULL) {
        instance = new TypeSystem();
    }
    return instance;
}

void TypeSystem::addStructToTable(StructDefine *s) {
    auto iter = structTable.find(s->getTag());
    if (iter != structTable.end()) {
        printf("Struct with name: %s is already defined", s->getTag().c_str());
    }
    structTable[s->getTag()] = s;
}

StructDefine * TypeSystem::getStructObjFromTable(string tag) {
    auto iter = structTable.find(tag);
    if (iter == structTable.end()) return NULL;
    return structTable[tag];
}

void TypeSystem::addSymbolsToTable(Symbol *headSymbol, string *scope) {
    while (headSymbol != NULL) {
        headSymbol->addScope(scope);
        auto iter = symbolTable.find(headSymbol->name);
        if (iter == symbolTable.end()) {
            symbolTable[headSymbol->name] = vector<Symbol *>();
            symbolTable[headSymbol->name].push_back(headSymbol);
        } else {
            handleDublicateSymbol(headSymbol, symbolTable[headSymbol->name]);
        }
        headSymbol = headSymbol->getNextSymbol();
    }
}

vector<Symbol *> * TypeSystem::getSymbol(string &text) {
    auto iter = symbolTable.find(text);
    if (iter != symbolTable.end()) {
        return &symbolTable[text];
    }
    return NULL;
}

void TypeSystem::handleDublicateSymbol(Symbol *symbol, vector<Symbol *> &symList) {
    bool harmless = true;
    auto iter = symList.begin();
    while (iter != symList.end()) {
        Symbol *sym = *iter;
        iter++;
        if (*sym == *symbol) {
            //同一层有两个同名的变量，就是出错了，需要处理这种情况
            //TODO, handle duplication here
            printf("Symbol definition replicate: %s", symbol->name.c_str());
            throw 0;
        }
    }
    if (harmless) {
        symList.push_back(symbol);
    }
}

void TypeSystem::removeSymbolFromTable(Symbol *symbol) {
    auto iter = symbolTable.find(symbol->name);
    if (iter == symbolTable.end()) return ;

    int pos = 0;
    while (pos < iter->second.size()) {
        Symbol *sym = iter->second.at(pos);
        if (sym->level == symbol->level) {
            std::remove(iter->second.begin(), iter->second.end(), sym);
            return;
        }
        pos++;
    }

}

TypeLink * TypeSystem::newType(string typeText) {
    Specifier *sp = NULL;
    int type = Specifier::CHAR;
    bool isLong = false, isSigned = true;
    switch (typeText.at(0)) {
        case 'c':
            if (typeText.at(1) == 'h') {
                type = Specifier::CHAR;
            }
            break;
        case 'd':
        case 'f':
            printf("No floating point support\n");

            break;
        case 'i':
            type = Specifier::INT;
            break;
        case 'l':
            isLong = true;
            break;
        case 'u':
            isSigned = false;
            break;
        case 'v':
            if (typeText.at(2) == 'i') {
                type = Specifier::VOID;
            }
            break;
        case 's':
            //ignore short signed
            break;
    }

    sp = new Specifier();
    sp->setType(type);
    sp->setLong(isLong);
    sp->setSign(isSigned);

    TypeLink *link = new TypeLink(false, false, sp);
    return link;
}

void TypeSystem::specifierCpy(Specifier *dst, Specifier *org) {
    dst->setConstantVal(org->getConstantVal());
    dst->setExternal(org->is_External());
    dst->setLong(org->getLong());
    dst->setOutputClass(org->getOutputClass());
    dst->setSign(org->isSign());
    dst->setStatic(org->is_Static());
    dst->setStorageClass(org->getStorageClass());
}

//遇到 static auto这种存储类型的关键字的时候，就要调newClass方法,与int long等不同需要调用setClassType设置
TypeLink * TypeSystem::newClass(string classText) {
    Specifier *sp = new Specifier();
    sp->setType(Specifier::NONE);
    setClassType(sp, classText.at(0));

    TypeLink *link = new TypeLink(false, false, sp);
    return link;
}

//如果是typedef 需要调用这个方法设置
//register int x
//typedef xxx
//static int x
//external
//读到这些声明，reduce时需要调用这个方法设置
void TypeSystem::setClassType(Specifier *sp, char c) {
    switch(c) {
        case 0:
            sp->setStorageClass(Specifier::FIXED);
            sp->setStatic(false);
            sp->setExternal(false);
            break;
        case 't':
            sp->setStorageClass(Specifier::TYPEDEF);
            break;
        case 'r':
            sp->setStorageClass(Specifier::REGISTER);
            break;
        case 's':
            sp->setStatic(true);
            break;
        case 'e':
            sp->setExternal(true);
            break;

        default:
            printf("Internal error, Invalid Class type\n");
            break;
    }
}

Symbol * TypeSystem::newSymbol(string name, int level) {
    return new Symbol(name, level);
}

Declarator * TypeSystem::addDeclarator(Symbol *symbol, int declaratorType) {
    Declarator *declarator = new Declarator(declaratorType);
    TypeLink *link = new TypeLink(true, false, declarator);
    symbol->addDeclarator(link);
    return declarator;
}

void TypeSystem::addSpecifierToDeclaration(TypeLink *specifier, Symbol *symbol) {
    while (symbol != NULL) {
        symbol->addSpecifier(specifier);
        symbol = symbol->getNextSymbol();
    }
}

Symbol * TypeSystem::getSymbolByText(string text, int level) {
    vector<Symbol *> *symbolList = getSymbol(text);
    int i = 0;
    Symbol *symbol = NULL;
    while (i < symbolList->size()) {
        symbol = symbolList->at(0);
        if (symbolList->at(i)->level == level) {
            return symbolList->at(i);
        } else if (symbolList->at(i)->getLevel() >= symbol->getLevel()) {
            symbol = symbolList->at(i);
        }
        i++;
    }

    return symbol;
}