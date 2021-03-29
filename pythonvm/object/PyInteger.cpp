//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyInteger.h"
#include "stdio.h"
#include "../runtime/universe.h"

IntegerKlass* IntegerKlass::instance = NULL;

IntegerKlass::IntegerKlass() {

}

IntegerKlass * IntegerKlass::get_instance() {
    if (instance == NULL) {
        instance = new IntegerKlass();
    }
    return instance;
}

void IntegerKlass::print(PyObject *x) {
    PyInteger* ix = (PyInteger*) x;
    assert(ix&&ix->klass() == (Klass* )this);
    printf("%d", ix->value());
}

PyObject * IntegerKlass::add(PyObject *x, PyObject* y) {
    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);
    return new PyInteger(ix->value() + iy->value());
}

PyObject * IntegerKlass::equal(PyObject *x, PyObject *y) {
    if (x->klass() != y->klass()) {
        return Universe::PyFalse;
    }

    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);

    if (ix->value() == iy->value()) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * IntegerKlass::less(PyObject *x, PyObject *y) {

    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);

    if (ix->value() < iy->value()) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * IntegerKlass::le(PyObject *x, PyObject *y) {
    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);

    if (ix->value() <= iy->value()) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * IntegerKlass::greater(PyObject *x, PyObject *y) {
    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);

    if (ix->value() > iy->value()) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * IntegerKlass::ge(PyObject *x, PyObject *y) {
    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);

    if (ix->value() >= iy->value()) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyObject * IntegerKlass::not_equal(PyObject *x, PyObject *y) {
    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);
    if (ix->value() != iy->value()) {
        return Universe::PyTrue;
    } else {
        return Universe::PyFalse;
    }
}

PyInteger::PyInteger(int v) {
    _value = v;
    set_kclass(IntegerKlass::get_instance());
}