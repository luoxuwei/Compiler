//
// Created by 罗旭维 on 2021/10/28.
//

#ifndef C_COMPILER_DIRECTMEMVALUESETTER_H
#define C_COMPILER_DIRECTMEMVALUESETTER_H
#include "IValueSetter.h"

class DirectMemValueSetter : public IValueSetter {
private:
    int memAddr;
public:
    DirectMemValueSetter(int addr) : memAddr(addr) {}
    void setValue(Value *v);
};


#endif //C_COMPILER_DIRECTMEMVALUESETTER_H
