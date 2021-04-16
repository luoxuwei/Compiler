//
// Created by Xuwei Luo 罗旭维 on 2021/3/31.
//

#ifndef PYTHONVM_PYLIST_H
#define PYTHONVM_PYLIST_H
#include "PyObject.h"
#include "../util/arraylist.h"

class ListKlass: public Klass {
private:
    static ListKlass* instance;
    ListKlass();

public:
    static ListKlass* get_instance();
    void initialize();
    virtual void print(PyObject* obj) override;
    virtual PyObject *subscr(PyObject* x, PyObject* y) override;
    virtual PyObject* contains(PyObject* x, PyObject* y) override;
    virtual void store_subscr(PyObject* x, PyObject* y, PyObject* z) override;
    virtual void delete_subscr(PyObject* x, PyObject* y) override;
    virtual PyObject *less(PyObject *x, PyObject *y) override;
    virtual PyObject* iter(PyObject* x) override;
    virtual PyObject *add(PyObject *x, PyObject *y) override;
};

class ListIteratorKlass: public Klass {
private:
    static ListIteratorKlass* instance;
    ListIteratorKlass();

public:
    static ListIteratorKlass* get_instance();
};

class PyList: public PyObject {
    friend class ListKlass;

private:
    ArrayList<PyObject*>* _inner_list;

public:
    PyList();
    PyList(ArrayList<PyObject*>* ol);
    ArrayList<PyObject*>* inner_list() {return _inner_list;}

    int size() {return _inner_list->size();}
    void append(PyObject* obj) {_inner_list->add(obj);}
    PyObject* pop() {return _inner_list->pop();}
    PyObject* get(int index) {return _inner_list->get(index);}
    void set(int i, PyObject* obj) {_inner_list->set(i, obj);}
    PyObject* top() {return _inner_list->get(_inner_list->size()-1);}
    void store_subscr(PyObject* x, PyObject* y, PyObject* z);
};

class ListIterator: public PyObject {
private:
    PyList* _owner;
    int _iter_cnt;
public:
    ListIterator(PyList* owner);
    PyList* owner() {return _owner;}
    int iter_cnt() {return _iter_cnt;}
    void inc_cnt() {_iter_cnt++;}
};

PyObject* listiterator_next(ArrayList<PyObject*>* args);

#endif //PYTHONVM_PYLIST_H
