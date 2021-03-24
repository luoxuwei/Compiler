//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_PYINTEGER_H
#define PYTHONVM_PYINTEGER_H
#include "PyObject.h"

class PyInteger: public PyObject {
private:
    int _value;
public:
    PyInteger(int v) {_value = v;}
    int value() {return _value;}
};


#endif //PYTHONVM_PYINTEGER_H
