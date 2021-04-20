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
    return instance;//天杀的，之前忘了加return！！跑起来就挂，调试了好久才发现！！！这样竟然也能编译通过还运行的了？！！！
}

DictKlass::DictKlass() {

}

void DictKlass::initialize() {
    set_name(new PyString("dict"));
}

void DictKlass::store_subscr(PyObject *x, PyObject *y, PyObject *z) {
    assert(x && x->klass() == this);
    ((PyDict*)x)->put(y, z);
}

PyObject * DictKlass::subscr(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    return ((PyDict*)x)->get(y);
}

void DictKlass::print(PyObject *x) {
    PyDict* dx = (PyDict*)x;
    assert(dx && dx->klass() == this);
    int size = dx->size();
    if (size >= 1) {
        dx->_map->entries()[0]._k->print();
        printf(":");
        dx->_map->entries()[0]._v->print();
    }

    for (int i=1; i<size; i++) {
        printf(",");
        dx->_map->entries()[i]._k->print();
        printf(":");
        dx->_map->entries()[i]._v->print();
    }
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