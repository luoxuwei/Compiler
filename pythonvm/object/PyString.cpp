//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyString.h"
#include "PyInteger.h"
#include <string.h>
#include <stdio.h>
#include "../runtime/universe.h"

PyString::PyString(const char *x) {
    _len = strlen(x);
    _value = new char[_len];
    strcpy(_value, x);
}

PyString::PyString(const char *x, const int len) {
    _len = len;
    _value = new char[len];
    //do not use strcpy here, since '\0' is allowed.
    for (int i=0; i<len; i++) {
        _value[i] = x[i];
    }
}

PyObject * PyString::add(PyObject *x) {
    int newLen = _len + ((PyString*) x)->_len;
    char* temp = new char[newLen];
    for (int i = 0; i<_len; i++) {
        temp[i] = _value[i];
    }
    for (int i = _len-1; i<_len; i++) {
        temp[i] = ((PyString*) x)->_value[i - _len + 1];
    }

    PyString* newStr = new PyString(temp, _len+((PyString*) x)->_len);
    delete[] temp;
    return newStr;
}

void PyString::print() {
    printf(_value);
}

//坑！！，忘记实现PyString::equal就直接在map里用作key的类型，导致map put时一比较就返回true，第一个就返回了，永远只能改第一个。
PyObject* PyString::equal(PyObject *x) {
    //TODO
    PyString* sx = (PyString*) x;
    if (_len != sx->length()) {
        return Universe::PyFalse;
    }
    for (int i=0; i<_len; i++) {
        if (_value[i] != sx->_value[i]) {
            return Universe::PyFalse;
        }
    }
    return Universe::PyTrue;
}

PyObject* PyString::less(PyObject *x) {
    if (true) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject* PyString::le(PyObject *x) {
    if (true) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * PyString::greater(PyObject *x) {
    if (true) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * PyString::ge(PyObject *x) {
    if (true) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * PyString::not_equal(PyObject *x) {
    if (true) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}