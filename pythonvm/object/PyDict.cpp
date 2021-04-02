//
// Created by Xuwei Luo 罗旭维 on 2021/3/30.
//

#include "PyDict.h"
#include "PyString.h"
DictKlass* DictKlass::instance = NULL;

DictKlass * DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }
}

DictKlass::DictKlass() {

}

void DictKlass::initialize() {
    set_name(new PyString("dict"));
}

PyDict::PyDict() {
    _map = new Map<PyObject*, PyObject*>();
    set_kclass(DictKlass::get_instance());
}

PyDict::PyDict(Map<PyObject*, PyObject*>* map) {
    _map = map;
    set_kclass(DictKlass::get_instance());
}

void PyDict::update(PyDict *d) {
    for (int i=0; i<d->size(); i++) {
        put(d->map()->get_key(i), d->map()->get_value(i));
    }
}