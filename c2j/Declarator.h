//
// Created by 罗旭维 on 2021/10/15.
//

#ifndef C_COMPILER_DECLARATOR_H
#define C_COMPILER_DECLARATOR_H
#include <map>
#include "Value.h"

class Declarator {
public:
    const static int POINTER = 0;
    const static int ARRAY = 1;
    const static int FUNCTION = 2;

    Declarator(int type);

    void setElementNum(int num);

    int getType();

    int getElementNum();

    void addElement(int index, Value *obj);

    Value * getElement(int index);

private:
    int declareType = POINTER;
    int numberOfElements = 0;
    //存数组元素的值,给数组元素赋值取值时用到
    map<int, Value *> elements;
};


#endif //C_COMPILER_DECLARATOR_H
