//
// Created by Xuwei Luo 罗旭维 on 2021/3/30.
//

#ifndef PYTHONVM_PYDICT_H
#define PYTHONVM_PYDICT_H
#include "PyObject.h"
#include "../util/map.h"
#include "../util/arraylist.h"

class DictKlass: public Klass {
private:
    static DictKlass* instance;
    DictKlass();

public:
    void initialize();
    static DictKlass* get_instance();
    virtual PyObject *subscr(PyObject* x, PyObject* y) override;
//    virtual PyObject* iter(PyObject* x) override;
    virtual void store_subscr(PyObject* x, PyObject* y, PyObject* z) override;
    virtual void print(PyObject* x) override;
    virtual void delete_subscr(PyObject* x, PyObject* y);
};

class PyDict: public PyObject {
friend class DictKlass;
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

PyObject* dict_set_default(ArrayList<PyObject*>* args);
PyObject* dict_remove(ArrayList<PyObject*>* args);

#endif //PYTHONVM_PYDICT_H
