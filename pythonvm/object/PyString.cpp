//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyString.h"
#include <string.h>

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