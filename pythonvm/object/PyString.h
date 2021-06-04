//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_PYSTRING_H
#define PYTHONVM_PYSTRING_H
#include "PyObject.h"
#include "Klass.h"

class StringKlass: public Klass {
private:
    StringKlass();
    static StringKlass* instance;

public:
    static StringKlass* get_instance();
    void initialize();
    virtual void print(PyObject* x);

    virtual PyObject *greater(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *less(PyObject *x, PyObject *y);

    virtual PyObject *equal(PyObject *x, PyObject *y);

    virtual PyObject *not_equal(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *ge(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *le(PyObject *x, PyObject *y) { return NULL; }


    virtual PyObject *add(PyObject *x, PyObject *y);

    virtual PyObject *sub(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *mul(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *div(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *mod(PyObject *x, PyObject *y) { return NULL; }

    virtual PyObject *len(PyObject *x);
    virtual PyObject *subscr(PyObject* x, PyObject* y) override;
    PyObject* contains(PyObject* x, PyObject* y);
    virtual PyObject* allocate_instance(PyObject* type_object, ArrayList<PyObject*>* args);
    size_t size() override;
    void oops_do(OopClosure *closure, PyObject* obj) override;
};

class PyString: public PyObject {
private:
    char* _value = NULL;
    unsigned int _len;
public:
    //可以使用strlen获取字符串长度
    PyString(const char* x);
    //'\0'也是允许的字符时不能使用strlen，只能通过参数传入长度
    PyString(const char* x, const int len);

    const char* value() {return _value;}
    char** value_address()  { return &_value; }
    int length() {return _len;}
};


#endif //PYTHONVM_PYSTRING_H
