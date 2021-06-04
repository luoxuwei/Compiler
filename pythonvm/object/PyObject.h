//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_JOBJECT_H
#define PYTHONVM_JOBJECT_H
#include "Klass.h"
#include "stdio.h"
#include "assert.h"
#include "../memory/oopClosure.h"

//python中所有数据都是对象，字符串/整数/浮点数/函数/模块/类定义等，它们都有共同的基类object。
class PyObject {
private:
    long _mark_world=0; //放在偏移为0的位置
    Klass* _klass = NULL;
    PyDict* _obj_dict = NULL;
public:
    PyObject();

    Klass* klass() {assert(_klass != NULL); return _klass;}
    void set_kclass(Klass* klass) { _klass = klass;}
    PyObject* setattr(PyObject* x, PyObject* y);
    PyDict* obj_dict() {return _obj_dict;}
    void init_dict();

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
    void store_subscr(PyObject* x, PyObject* y);
    void delete_subscr(PyObject* x);
    PyObject* iter();
    void oops_do(OopClosure* oopClosure);
    size_t size();
    char* new_address();
    void set_new_address(char* addr);
    //c++中使用new来创建对象有三个步骤:1.分配内存;2.调用A()构造对象;3.返回分配指针.
    //分配内存这个操作就是由 operator new(size_t)来完成的，如果类A重载了operator new
    //那么将调用A::operator new(size_t)，否则调用全局"::operator new(size_t)",后者
    //由c++默认提供。new带的参数就是sizeof(A)，通过这种方式，所有PyObject的子类在实例化的时候
    //都会通过虚拟机的堆分配内存。也就是说，所有的Python对象全部都已经被管理起来了。
    void* operator new(size_t size);
};

class ObjectKlass : public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;

public:
    static ObjectKlass* get_instance();
    void initialize();
};

#endif //PYTHONVM_JOBJECT_H
