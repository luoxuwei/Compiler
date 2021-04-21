//
// Created by Xuwei Luo 罗旭维 on 2021/3/30.
//
#include "../runtime/universe.h"
#include "PyDict.h"
#include "PyString.h"
#include "../runtime/FunctionObject.h"
#include "PyList.h"
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
    PyDict* klass_dict = new PyDict();
    klass_dict->put(new PyString("setdefault"), new FunctionObject(dict_set_default));
    klass_dict->put(new PyString("remove"), new FunctionObject(dict_remove));
    klass_dict->put(new PyString("keys"), new FunctionObject(dict_keys));
    klass_dict->put(new PyString("values"), new FunctionObject(dict_values));
    set_klass_dict(klass_dict);
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

void DictKlass::delete_subscr(PyObject *x, PyObject *y) {
    PyDict* dict = (PyDict*) x;
    assert(x && x->klass() == this);
    dict->remove(y);
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

PyObject* dict_set_default(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyObject* key = args->get(1);
    PyObject* value = args->get(2);
    if (!dict->has_key(key)) {
        dict->put(key, value);
    }
    return Universe::PyNone;
}

PyObject* dict_remove(ArrayList<PyObject*>* args) {

    PyDict* dict = (PyDict*) args->get(0);
    PyObject* key = args->get(1);
    dict->remove(key);
    return Universe::PyNone;
}

PyObject* dict_keys(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyList* list = new PyList();
    for (int i = 0; i<dict->size(); i++) {
        list->append(dict->map()->entries()[i]._k);
    }
    return list;
}

PyObject* dict_values(ArrayList<PyObject*>* args) {
    PyDict* dict = (PyDict*) args->get(0);
    PyList* list = new PyList();
    for (int i = 0; i<dict->size(); i++) {
        list->append(dict->map()->entries()[i]._v);
    }
    return list;
}
