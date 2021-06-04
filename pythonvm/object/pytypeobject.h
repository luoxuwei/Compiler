//
// Created by Xuwei Luo 罗旭维 on 2021/5/13.
//

#ifndef PYTHONVM_PYTYPEOBJECT_H
#define PYTHONVM_PYTYPEOBJECT_H
#include "PyObject.h"
#include "Klass.h"
//python中的类型 list str dict 既是类又是对象所以像print(list)是能正常执行的，list代表列表类，同时又是一个对象，所以能被输出
//但它又与我们以前所接触的所有对象都不同，它代表了一个类型。可以在klass中引入一个PyObject，让klass与这个PyObject一一对应，当需要它
//是一个对象的时候，比如要输出输出这个对象修改这个对象的属性等，就让PyObject出面。当需要它是一个类型的时候，比如创建该类型的对象，调用
//该类型上定义的某个方法就让klass出面。这种组合的设计与Klass-Oop二分结构一致。
class PyTypeObject: public PyObject {
private:
    Klass* _own_klass = NULL;
public:
    PyTypeObject();
    void set_own_klass(Klass* k);
    Klass* own_klass() {return _own_klass;}
};

class TypeKlass: public Klass {
private:
    TypeKlass() {}
    static TypeKlass* _instance;
public:
    static TypeKlass* get_instance();
    virtual void print(PyObject* obj);
    void initialize();
    virtual PyObject* setattr(PyObject* x, PyObject* y, PyObject* z) override;
    size_t size() override;
};


#endif //PYTHONVM_PYTYPEOBJECT_H
