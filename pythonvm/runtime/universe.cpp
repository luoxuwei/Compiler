//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#include "universe.h"
#include <stdio.h>
#include "../object/PyString.h"

PyObject* Universe::PyFalse = NULL;
PyObject* Universe::PyTrue = NULL;
PyObject* Universe::PyNone = NULL;

void Universe::genesis() {
    PyFalse = new PyString("False");
    PyTrue = new PyString("True");
    PyNone = new PyString("None");
}

void Universe::destroy() {

}