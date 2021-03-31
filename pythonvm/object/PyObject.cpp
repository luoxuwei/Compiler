//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyObject.h"
#include "stdio.h"
#include "PyDict.h"
#include "../runtime/universe.h"
#include "../runtime/FunctionObject.h"

PyObject::PyObject() {

}

void PyObject::print() {
    _klass->print(this);
}

PyObject* PyObject::add(PyObject *x) {
    return _klass->add(this, x);
}

PyObject *PyObject::sub(PyObject *x) {
    return _klass->sub(this, x);
}

PyObject *PyObject::mul(PyObject *x) {
    return _klass->mul(this, x);
}

PyObject *PyObject::div(PyObject *x) {
    return _klass->div(this, x);
}

PyObject *PyObject::mod(PyObject *x) {
    return _klass->mod(this, x);
}

PyObject* PyObject::greater(PyObject* x)  {
    return _klass->greater(this, x);
}
PyObject* PyObject::less(PyObject* x) {
    return _klass->less(this, x);
}
PyObject* PyObject::equal(PyObject* x) {
    return _klass->equal(this, x);
}
PyObject* PyObject::not_equal(PyObject* x) {
    return _klass->not_equal(this, x);
}
PyObject* PyObject::ge(PyObject* x) {
    return _klass->ge(this, x);
}
PyObject* PyObject::le(PyObject* x) {
    return _klass->le(this, x);
}

PyObject * PyObject::len() {
    return _klass->len(this);
}

PyObject * PyObject::getattr(PyObject *k) {
    PyObject* result = klass()->klass_dict()->get(k);
    if (result == Universe::PyNone) {
        return result;
    }
//    if (MethodObject::is_function(result)) {
//    }
    result = new MethodObject((FunctionObject*)result, this);
    return result;
}

PyObject * PyObject::subscr(PyObject *x) {
    return klass()->subscr(this, x);
}