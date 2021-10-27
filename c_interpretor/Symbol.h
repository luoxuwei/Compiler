//
// Created by 罗旭维 on 2021/10/14.
//

#ifndef C_COMPILER_SYMBOL_H
#define C_COMPILER_SYMBOL_H
#include <string>
#include "TypeLink.h"
#include "Declarator.h"
#include "Value.h"
#include "IValueSetter.h"


using namespace std;
class Symbol : public IValueSetter {
private:

    string rname;//代码生成时用到，如果生成汇编，就是对应寄存器的名称


    bool implicit;//是否是匿名变量
    bool duplicate;//是否是同名变量，不同层次可以定义相同名称的变量，只是他们的声明周期不一样

    Symbol *next = NULL;//指向下一个同层次的变量符号

    TypeLink *typeLinkBegin = NULL;
    TypeLink *typeLinkEnd = NULL;
    Value *value = NULL;


public:

    string *symbolScope = NULL;
    string name;
    int level; //变量的层次，嵌套局部变量时用到，局部变量和全局变量就是用层次来表示
    Symbol *args = NULL;   //如果该符号对应的是函数名,那么args指向函数的输入参数符号列表
    Symbol(string name, int level);

    void addDeclarator(TypeLink *type);
    void addSpecifier(TypeLink *type);
    void setNextSymbol(Symbol *symbol);
    Symbol * getNextSymbol();
    TypeLink *getTypeHead() {return typeLinkBegin;};
    string *getName() {return &name;}
    void setValue(Value *obj) override;
    Value *getValue() {return value;}
    int getLevel() {return level;}
    Declarator *getDeclarator(int type);
    void addScope(string *scope);
    bool operator==(const Symbol &s);
    Symbol * getArgList() {return args;}
    string * getScope() {return symbolScope;}
};


#endif //C_COMPILER_SYMBOL_H
