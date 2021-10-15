//
// Created by 罗旭维 on 2021/10/15.
//

#ifndef C_COMPILER_STRUCTDEFINE_H
#define C_COMPILER_STRUCTDEFINE_H

#include <string>
#include "Symbol.h"
/*
 * struct argotiers {
 *     int  (*Clopin)();
 *     double  Mathias[5];
 *     struct  argotiers*  Guillaume;
 *     struct  pstruct {int a;} Pierre;
 * }
 */
using namespace std;
class StructDefine {
private:
    string tag; //结构体的名称,例如上面的例子中，对应该变量的值为 "argotiers"
    int  level; //结构体的间套层次
    Symbol *fields; //对应结构体里的各个变量类型

public:
    StructDefine(string tag, int level, Symbol *fields) : tag(tag), level(level),  fields(fields) {}

    string getTag() {
        return tag;
    }

    int getLevel() {
        return level;
    }

    Symbol * getFields() {
        return fields;
    }
};


#endif //C_COMPILER_STRUCTDEFINE_H
