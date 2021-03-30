//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#include "universe.h"
#include <stdio.h>
#include "../object/PyString.h"
#include "../object/PyDict.h"
#include "../runtime/FunctionObject.h"

PyObject* Universe::PyFalse = NULL;
PyObject* Universe::PyTrue = NULL;
PyObject* Universe::PyNone = NULL;

void Universe::genesis() {
    PyFalse = new PyString("False");
    PyTrue = new PyString("True");
    PyNone = new PyString("None");

    PyDict* klass_dict = new PyDict();
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    klass_dict->put(new PyString("upper"), new FunctionObject(string_upper));
}

void Universe::destroy() {

}