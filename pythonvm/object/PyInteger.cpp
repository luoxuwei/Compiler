//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyInteger.h"
#include "stdio.h"
#include "../runtime/universe.h"
#include "../object/PyString.h"
#include "pytypeobject.h"

IntegerKlass* IntegerKlass::instance = NULL;

IntegerKlass::IntegerKlass() {
}

IntegerKlass * IntegerKlass::get_instance() {
    if (instance == NULL) {
        instance = new IntegerKlass();
    }
    return instance;
}

void IntegerKlass::initialize() {
    (new PyTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
    set_name(new PyString("int"));
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

    assert(ix&&ix->klass() == (Klass* )this);
    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0) {
            return Universe::PyTrue;
        } else {
            return Universe::PyFalse;
        }
    }
    PyInteger* iy = (PyInteger*) y;
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

PyObject * IntegerKlass::allocate_instance(PyObject* type_object, ArrayList<PyObject *> *args) {
    if (!args || args->length() == 0) {
        return new PyInteger(0);
    } else {
        return NULL;
    }
}