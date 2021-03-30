//
// Created by Xuwei Luo 罗旭维 on 2021/3/30.
//

#ifndef PYTHONVM_PYDICT_H
#define PYTHONVM_PYDICT_H
#include "PyObject.h"
#include "../util/map.h"

class DictKlass: public Klass {
private:
    static DictKlass* instance;
    DictKlass();

public:
    static DictKlass* get_instance();
};

class PyDict: public PyObject {
private:
    Map<PyObject*, PyObject*>* _map;
public:
    PyDict();
    PyDict(Map<PyObject*, PyObject*>* map);
    Map<PyObject*, PyObject*>* map() {return _map;}
    void put(PyObject* k, PyObject* v) { _map->put(k, v);}
    PyObject* get(PyObject* k) { return _map->get(k); }
    bool has_key(PyObject* k) { return _map->has_key(k); }
    int size() { return _map->size(); }
    PyObject* remove(PyObject* k) { return _map->remove(k);}
    void update(PyDict* d);

};


#endif //PYTHONVM_PYDICT_H
