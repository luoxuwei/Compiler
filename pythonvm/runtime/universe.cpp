//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#include "universe.h"
#include <stdio.h>

PyInteger* Universe::PyFalse = NULL;
PyInteger* Universe::PyTrue = NULL;
PyObject* Universe::PyNone = NULL;

void Universe::genesis() {
    PyFalse = new PyInteger(0);
    PyTrue = new PyInteger(1);
    PyNone = new PyObject();
}

void Universe::destroy() {

}