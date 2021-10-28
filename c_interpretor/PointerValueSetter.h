//
// Created by 罗旭维 on 2021/10/28.
//

#ifndef C_COMPILER_POINTERVALUESETTER_H
#define C_COMPILER_POINTERVALUESETTER_H
#include "IValueSetter.h"
#include "Symbol.h"

class PointerValueSetter : public IValueSetter {
private:
    Symbol *symbol = NULL;
    int index = 0;
public:
    PointerValueSetter(Symbol *s, int i);
    void setValue(Value *v);
};


#endif //C_COMPILER_POINTERVALUESETTER_H
