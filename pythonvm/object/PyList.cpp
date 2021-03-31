//
// Created by Xuwei Luo 罗旭维 on 2021/3/31.
//

#include "PyList.h"
#include "PyInteger.h"
ListKlass* ListKlass::instance = NULL;
ListKlass * ListKlass::get_instance() {
    if (instance == NULL) {
        instance = new ListKlass();
    }
    return instance;
}
ListKlass::ListKlass() {

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

PyList::PyList() {
    _inner_list = new ArrayList<PyObject*>();
    set_kclass(ListKlass::get_instance());
}

PyList::PyList(ArrayList<PyObject *> *ol) {
    _inner_list = ol;
    set_kclass(ListKlass::get_instance());
}

