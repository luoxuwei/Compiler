//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyInteger.h"
#include "stdio.h"
#include "../runtime/universe.h"

IntegerKclass* IntegerKclass::instance = NULL;

IntegerKclass::IntegerKclass() {

}

IntegerKclass * IntegerKclass::get_instance() {
    if (instance == NULL) {
        instance = new IntegerKclass();
    }
    return instance;
}

void IntegerKclass::print(PyObject *x) {
    PyInteger* ix = (PyInteger*) x;
    assert(ix&&ix->klass() == (Klass* )this);
    printf("%d", ix->value());
}

PyObject * IntegerKclass::add(PyObject *x, PyObject* y) {
    PyInteger* ix = (PyInteger*) x;
    PyInteger* iy = (PyInteger*) y;

    assert(ix&&ix->klass() == (Klass* )this);
    assert(iy&&iy->klass() == (Klass* )this);
    return new PyInteger(ix->value() + iy->value());
}

PyObject * IntegerKclass::equal(PyObject *x, PyObject *y) {
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

PyObject * IntegerKclass::less(PyObject *x, PyObject *y) {

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

PyObject * IntegerKclass::le(PyObject *x, PyObject *y) {
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

PyObject * IntegerKclass::greater(PyObject *x, PyObject *y) {
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

PyObject * IntegerKclass::ge(PyObject *x, PyObject *y) {
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

PyObject * IntegerKclass::not_equal(PyObject *x, PyObject *y) {
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
    set_kclass(IntegerKclass::get_instance());
}