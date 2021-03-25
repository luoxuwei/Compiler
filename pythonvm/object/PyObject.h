//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_JOBJECT_H
#define PYTHONVM_JOBJECT_H

//python中所有数据都是对象，字符串/整数/浮点数/函数/模块/类定义等，它们都有共同的基类object。
class PyObject {
public:
    virtual void print();
    virtual PyObject* add(PyObject* x);
    virtual PyObject* greater(PyObject* x);
    virtual PyObject* less(PyObject* x);
    virtual PyObject* equal(PyObject* x);
    virtual PyObject* not_equal(PyObject* x);
    virtual PyObject* ge(PyObject* x);
    virtual PyObject* le(PyObject* x);
};

#endif //PYTHONVM_JOBJECT_H
