//
// Created by 罗旭维 on 2021/10/15.
//

#include "Declarator.h"

Declarator::Declarator(int type) : declareType(type) {
    if (type < POINTER) {
        declareType = type;
    }

    if (type > FUNCTION) {
        type = FUNCTION;
    } else {
        declareType = type;
    }
}

int Declarator::getElementNum() {return numberOfElements;}

int Declarator::getType() {return declareType;}

void Declarator::setElementNum(int num) {
    numberOfElements = num;
    if (num < 0) {
        numberOfElements = 0;
    }
}

void Declarator::addElement(int index, Value *obj) {
    if (index >= numberOfElements) {
        throw "Array out of Range";
    }

    elements[index] = obj;
}

Value * Declarator::getElement(int index) {
    if (index >= numberOfElements) {
        throw "index out of range";
    }

    return elements[index];
}
