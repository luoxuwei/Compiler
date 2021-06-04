//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyObject.h"
#include "stdio.h"
#include "PyDict.h"
#include "../runtime/universe.h"
#include "../runtime/FunctionObject.h"
#include "pytypeobject.h"
#include "../memory/heap.h"

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

//没有直接赋值，而是将地址与2做了二进制"或运算"以后在赋值，这样做的目的是对forwarding指针加以区分。
//堆中的地址都是8字节对齐的所以指向它们的指针的低3位就一定是0，在这里把从低位开始的第2位置为1是完全没问题的。
//同样在new_address方法中，如果发现了当前的forwarding不为空，则需要将这个指针的低3位重置为0。也就是和"-8"进行"与操作"。
void PyObject::set_new_address(char *addr) {
    if (!addr)
        return;
    _mark_world = ((long)addr) | 0x2;
}

char * PyObject::new_address() {
    if ((_mark_world & 0x2) == 0x2)
        return (char*)(_mark_world&((long)-8));

    return NULL;
}

size_t PyObject::size() {
    return klass()->size();
}

void * PyObject::operator new(size_t size) {
    return Universe::heap->allocate(size);
}

void PyObject::oops_do(OopClosure *oopClosure) {
    oopClosure->do_oop((PyObject**)&_obj_dict);
    klass()->oops_do(oopClosure);
}