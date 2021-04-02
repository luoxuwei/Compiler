//
// Created by Xuwei Luo 罗旭维 on 2021/3/31.
//

#include "PyList.h"
#include "PyInteger.h"
#include "../runtime/universe.h"
#include "PyDict.h"
#include "PyString.h"
#include "../runtime/FunctionObject.h"

ListKlass* ListKlass::instance = NULL;
ListKlass * ListKlass::get_instance() {
    if (instance == NULL) {
        instance = new ListKlass();
    }
    return instance;
}
ListKlass::ListKlass() {

}

void ListKlass::initialize() {
    PyDict* klass_dict = new PyDict();
    klass_dict->put(new PyString("append"), new FunctionObject(list_append));
    klass_dict->put(new PyString("index"), new FunctionObject(list_index));
    klass_dict->put(new PyString("pop"), new FunctionObject(list_pop));
    klass_dict->put(new PyString("remove"), new FunctionObject(list_remove));
    klass_dict->put(new PyString("reverse"), new FunctionObject(list_reverse));
    klass_dict->put(new PyString("sort"), new FunctionObject(list_sort));

    set_klass_dict(klass_dict);
    set_name(new PyString("list"));
}

void ListKlass::print(PyObject *obj) {
    assert(obj && obj->klass() == this);
    PyList* pyList = (PyList*)obj;
    printf("[");
    int size = pyList->size();
    if (size >= 1) {
        pyList->get(0)->print();
    }
    for(int i=1; i<size; i++) {
        printf(",");
        pyList->get(i)->print();
    }
    printf("]");
}

PyObject * ListKlass::subscr(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());
    PyList* lx = (PyList*) x;
    PyInteger* iy = (PyInteger*) y;
    return lx->get(iy->value());
}

PyObject * ListKlass::contains(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    PyList* lx = (PyList*)x;
    int size = lx->size();
    for (int i=0; i<size; i++) {
        if (lx->get(i)->equal(y) == Universe::PyTrue) {
            return Universe::PyTrue;
        }
    }
    return Universe::PyFalse;
}

void ListKlass::store_subscr(PyObject *x, PyObject *y, PyObject *z) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());
    PyList* lx = (PyList*)x;
    PyInteger* iy = (PyInteger*)y;
    lx->set(iy->value(), z);
}

void ListKlass::delete_subscr(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());
    PyList* list = (PyList*) x;
    PyInteger* index = (PyInteger*) y;
    list->inner_list()->delete_index(index->value());
}

PyObject * ListKlass::less(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0) {
            return Universe::PyTrue;
        } else {
            return Universe::PyFalse;
        }
    }

    PyList* lx = (PyList*) x;
    PyList* ly = (PyList*) y;
    assert(y && y->klass() == ListKlass::get_instance());
    int len = lx->size() < ly->size()?lx->size():ly->size();
    for (int i=0; i<len; i++) {
        if (lx->get(i)->less(ly->get(i)) == Universe::PyTrue) {
            return Universe::PyTrue;
        } else if (ly->get(i)->less(lx->get(i)) == Universe::PyTrue) {
            return Universe::PyFalse;
        }
    }
    if (lx->size() < ly->size()) {
        return Universe::PyTrue;
    }
    return Universe::PyFalse;

}

PyList::PyList() {
    _inner_list = new ArrayList<PyObject*>();
    set_kclass(ListKlass::get_instance());
}

PyList::PyList(ArrayList<PyObject *> *ol) {
    _inner_list = ol;
    set_kclass(ListKlass::get_instance());
}

