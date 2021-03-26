//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_PYINTEGER_H
#define PYTHONVM_PYINTEGER_H
#include "PyObject.h"

class IntegerKclass: public Klass {
private:
    IntegerKclass();
    static IntegerKclass* instance;

public:
    static IntegerKclass* get_instance();
    virtual void print(PyObject* x);

    virtual PyObject *greater(PyObject *x, PyObject *y);

    virtual PyObject *less(PyObject *x, PyObject *y);

    virtual PyObject *equal(PyObject *x, PyObject *y);

    virtual PyObject *not_equal(PyObject *x, PyObject *y);

    virtual PyObject *ge(PyObject *x, PyObject *y);

    virtual PyObject *le(PyObject *x, PyObject *y);


    virtual PyObject *add(PyObject *x, PyObject *y);

    virtual PyObject *sub(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *mul(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *div(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *mod(PyObject *x, PyObject *y) { return NULL; }
};

class PyInteger: public PyObject {
private:
    int _value;
public:
    PyInteger(int v);
    int value() {return _value;}
};


#endif //PYTHONVM_PYINTEGER_H
