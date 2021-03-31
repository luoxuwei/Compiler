//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_JOBJECT_H
#define PYTHONVM_JOBJECT_H
#include "Klass.h"
#include "stdio.h"
#include "assert.h"
//python中所有数据都是对象，字符串/整数/浮点数/函数/模块/类定义等，它们都有共同的基类object。
class PyObject {
private:
    Klass* _klass;
public:
    PyObject();

    Klass* klass() {assert(_klass != NULL); return _klass;}
    void set_kclass(Klass* klass) { _klass = klass;}

    void print();

    PyObject *greater(PyObject *x);

    PyObject *less(PyObject *x);

    PyObject *equal(PyObject *x);

    PyObject *not_equal(PyObject *x);

    PyObject *ge(PyObject *x);

    PyObject *le(PyObject *x);



    PyObject *add(PyObject *x);

    PyObject *sub(PyObject *x);

    PyObject *mul(PyObject *x);

    PyObject *div(PyObject *x);

    PyObject *mod(PyObject *x);
    PyObject* len();
    PyObject* getattr(PyObject* k);
    PyObject* subscr(PyObject* x);
    PyObject* contains(PyObject* x);
};

#endif //PYTHONVM_JOBJECT_H
