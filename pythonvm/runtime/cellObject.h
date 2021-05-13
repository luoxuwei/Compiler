//
// Created by Xuwei Luo 罗旭维 on 2021/5/13.
//

#ifndef PYTHONVM_CELLOBJECT_H
#define PYTHONVM_CELLOBJECT_H
#include "../object/PyObject.h"
#include "../object/PyList.h"

class CellKlass: public Klass {
private:
    CellKlass();
    static CellKlass* _instance;
public:
    static CellKlass* getInstance();

    virtual size_t size();
};

//为了实现LOAD_CLOSURE字节码需要一种新的数据结构，记录cell变量所在表和表中的序号。
//如果直接把cell变量传到内部函数里，那么当在外部修改过这个变量的值，内部函数就无法看到了，
//这本质是传值和传引用的区别
class CellObject: public PyObject {
    friend class CellKlass;

private:
    PyList* _table;//记录该cell变量所在的closure表
    int _index;//记录cell变量在表中的序号
    //这样做的话如果closure表中的变量被STORE_CLOSURE修改过，通过CellObject仍然可以访问到
public:
    CellObject(PyList* l, int i);
    PyObject* value();

};


#endif //PYTHONVM_CELLOBJECT_H
