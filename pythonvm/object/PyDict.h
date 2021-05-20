//
// Created by Xuwei Luo 罗旭维 on 2021/3/30.
//

#ifndef PYTHONVM_PYDICT_H
#define PYTHONVM_PYDICT_H
#include "PyObject.h"
#include "../util/map.h"
#include "../util/arraylist.h"
enum ITER_TYPE {
    ITER_KEY=0,
    ITER_VALUE,
    ITER_ITEM
};

template<ITER_TYPE n>
class DictIteratorKlass: public Klass {
private:
    static DictIteratorKlass* instance;
    DictIteratorKlass();

public:
    static DictIteratorKlass* get_instance();
    virtual PyObject* iter(PyObject* x) override {return x;}//入参实际上是一个迭代器对象
    virtual PyObject* next(PyObject* x);
};
class DictIterator: public PyObject {
private:
    PyDict* _owner;
    int _iter_cnt;
public:
    DictIterator(PyDict* owner);
    PyDict* owner() {return _owner;}
    int iter_cnt() {return _iter_cnt;}
    void inc_cnt() {_iter_cnt++;}
};

class DictKlass: public Klass {
private:
    static DictKlass* instance;
    DictKlass();

public:
    void initialize();
    static DictKlass* get_instance();
    virtual PyObject *subscr(PyObject* x, PyObject* y) override;
    virtual PyObject* iter(PyObject* x) override;
    virtual void store_subscr(PyObject* x, PyObject* y, PyObject* z) override;
    virtual void print(PyObject* x) override;
    virtual void delete_subscr(PyObject* x, PyObject* y);
    virtual PyObject* allocate_instance(PyObject* type_object, ArrayList<PyObject*>* args);
};

class PyDict: public PyObject {
friend class DictKlass;
private:
    Map<PyObject*, PyObject*>* _map = NULL;
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
PyObject* dict_keys(ArrayList<PyObject*>* args);
PyObject* dict_values(ArrayList<PyObject*>* args);
PyObject* dict_items(ArrayList<PyObject*>* args);
PyObject* dict_iterkeys(ArrayList<PyObject*>* args);
PyObject* dict_iteritems(ArrayList<PyObject*>* args);
PyObject* dict_itervalues(ArrayList<PyObject*>* args);
PyObject* dict_iternext(ArrayList<PyObject*>* args);
#endif //PYTHONVM_PYDICT_H
