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

//python中true和false也是对象，这里就用1代表true，0代表false
PyObject* PyInteger::equal(PyObject *x) {
   if (_value == ((PyInteger *) x)->_value) {
       return new PyInteger(1);
   } else {
       return new PyInteger(0);
   }
}

PyObject* PyInteger::less(PyObject *x) {
    if (_value < ((PyInteger *) x)->_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject* PyInteger::le(PyObject *x) {
    if (_value <= ((PyInteger *) x)->_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject * PyInteger::greater(PyObject *x) {
    if (_value > ((PyInteger *) x)->_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject * PyInteger::ge(PyObject *x) {
    if (_value >= ((PyInteger *) x)->_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}

PyObject * PyInteger::not_equal(PyObject *x) {
    if (_value != ((PyInteger *) x)->_value) {
        return new PyInteger(1);
    } else {
        return new PyInteger(0);
    }
}