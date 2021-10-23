//
// Created by 罗旭维 on 2021/10/22.
//

#ifndef C_COMPILER_ARRAYVALUESETTER_H
#define C_COMPILER_ARRAYVALUESETTER_H
#include "IValueSetter.h"
#include "Symbol.h"

//封装数组元素赋值操作
class ArrayValueSetter : public IValueSetter {
private:
    Symbol *symbol;
    int index;

public:
    ArrayValueSetter(Symbol *s, int i);
    void setValue(Value *v) override;
};


#endif //C_COMPILER_ARRAYVALUESETTER_H
