//
// Created by 罗旭维 on 2021/10/15.
//

#ifndef C_COMPILER_TYPESYSTEM_H
#define C_COMPILER_TYPESYSTEM_H
#include <map>
#include <string>
#include <vector>
#include "Symbol.h"
#include "Specifier.h"
#include "StructDefine.h"

using namespace std;
class TypeSystem {
private:
    map<string, vector<Symbol *>> symbolTable;
    map<string, StructDefine *> structTable;

    void handleDublicateSymbol(Symbol *symbol, vector<Symbol *> &symList);
    void setClassType(Specifier *sp, char c);

public:
    void addSymbolsToTable(Symbol * headSymbol);
    TypeLink * newType(string typeText);
    void specifierCpy(Specifier *dst, Specifier *org);
    TypeLink * newClass(string classText);
    Symbol * newSymbol(string name, int level);
    void addDeclarator(Symbol *symbol, int declaratorType);
    void addSpecifierToDeclaration(TypeLink * specifier, Symbol * symbol);
    void addStructToTable(StructDefine *s);
    StructDefine * getStructObjFromTable(string tag);
};


#endif //C_COMPILER_TYPESYSTEM_H
