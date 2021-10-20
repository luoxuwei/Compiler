//
// Created by 罗旭维 on 2021/10/14.
//

#ifndef C_COMPILER_SYMBOL_H
#define C_COMPILER_SYMBOL_H
#include <string>
#include "Symbol.h"
#include "TypeLink.h"

using namespace std;
class Symbol {
private:

    string rname;//代码生成时用到，如果生成汇编，就是对应寄存器的名称


    bool implicit;//是否是匿名变量
    bool duplicate;//是否是同名变量，不同层次可以定义相同名称的变量，只是他们的声明周期不一样

    Symbol *next = NULL;//指向下一个同层次的变量符号

    TypeLink *typeLinkBegin = NULL;
    TypeLink *typeLinkEnd = NULL;
    void *value = NULL;

public:
    string name;
    int level; //变量的层次，嵌套局部变量时用到，局部变量和全局变量就是用层次来表示
    Symbol *args;   //如果该符号对应的是函数名,那么args指向函数的输入参数符号列表
    Symbol(string name, int level);

    void addDeclarator(TypeLink *type);
    void addSpecifier(TypeLink *type);
    void setNextSymbol(Symbol *symbol);
    Symbol * getNextSymbol();
    TypeLink *getTypeHead() {return typeLinkBegin;};
    string getName() {return name;}
    void setValue(void *obj) {value = obj;}
    void *getValue() {return value;}
    int getLevel() {return level;}
};


#endif //C_COMPILER_SYMBOL_H
