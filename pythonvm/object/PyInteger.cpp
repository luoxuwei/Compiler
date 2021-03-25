//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyInteger.h"
#include "stdio.h"

PyObject * PyInteger::add(PyObject *x)  {
    return new PyInteger(_value + ((PyInteger*) x)->_value);
}

void PyInteger::print() {
    printf("%d", _value);
}