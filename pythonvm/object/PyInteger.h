//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_PYINTEGER_H
#define PYTHONVM_PYINTEGER_H
#include "PyObject.h"

class PyInteger: public PyObject {
private:
    int _value;
public:
    PyInteger(int v) {_value = v;}
    int value() {return _value;}
    virtual void print() override;
    virtual PyObject* add(PyObject* x) override;

    virtual PyObject* greater(PyObject* x) override;
    virtual PyObject* less(PyObject* x) override;
    virtual PyObject* equal(PyObject* x) override;
    virtual PyObject* not_equal(PyObject* x) override;
    virtual PyObject* ge(PyObject* x) override;
    virtual PyObject* le(PyObject* x) override;
};


#endif //PYTHONVM_PYINTEGER_H
