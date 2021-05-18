//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyObject.h"
#include "stdio.h"
#include "PyDict.h"
#include "../runtime/universe.h"
#include "../runtime/FunctionObject.h"
#include "pytypeobject.h"

ObjectKlass* ObjectKlass::instance = NULL;

ObjectKlass * ObjectKlass::get_instance() {
    if (instance == NULL) {
        instance = new ObjectKlass();
    }
    return instance;
}

ObjectKlass::ObjectKlass() {
}

void ObjectKlass::initialize() {
    (new PyTypeObject())->set_own_klass(this);
    set_name(new PyString("object"));
}

PyObject::PyObject() {
    set_kclass(ObjectKlass::get_instance());
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

//获取类的属性，包括方法和字段
PyObject * PyObject::getattr(PyObject *k) {
    return klass()->getattr(this, k);
}

PyObject * PyObject::subscr(PyObject *x) {
    return klass()->subscr(this, x);
}

PyObject * PyObject::contains(PyObject *x) {
    return klass()->contains(this, x);
}

void PyObject::store_subscr(PyObject *x, PyObject *y) {
    klass()->store_subscr(this, x, y);
}

void PyObject::delete_subscr(PyObject *x) {
    klass()->delete_subscr(this, x);
}

PyObject * PyObject::iter() {
    return klass()->iter(this);
}

PyObject * PyObject::setattr(PyObject *x, PyObject *y) {
    return klass()->setattr(this, x, y);
}

void PyObject::init_dict() {
    _obj_dict = new PyDict();
}