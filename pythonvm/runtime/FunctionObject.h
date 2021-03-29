//
// Created by Xuwei Luo 罗旭维 on 2021/3/29.
//

#ifndef PYTHONVM_FUNCTIONOBJECT_H
#define PYTHONVM_FUNCTIONOBJECT_H
#include "../object/Klass.h"
#include "../code/CodeObject.h"
#include "../util/map.h"

class FunctionKlass: public Klass {
private:
    static FunctionKlass* _instance;
    Map<PyObject*, PyObject*>* _globals;

    FunctionKlass();

public:
    static FunctionKlass* get_instance();
    virtual void print(PyObject* x) override;
};

class FunctionObject: public PyObject {
    friend class FunctionKlass;
    friend class FrameObject;
private:
    CodeObject* _func_code;
    PyString* _func_name;
    unsigned int _flags;
    //函数所依赖的全局变量是定义函数对象的时候，而不是调用函数的时候。这就要求必须为FunctionObject对象引入一个global变量表。
    Map<PyObject*, PyObject*>* _globals;
public:
    FunctionObject(PyObject* code_object);
    FunctionObject(Klass* klass) {
        _func_code = NULL;
        _func_name = NULL;
        _flags = 0;
        set_kclass(klass);
    }

    PyString* func_name() {return _func_name;}
    unsigned int flags() {return _flags;}
    Map<PyObject*, PyObject*>* globals() {return _globals;}
    void set_gloabls(Map<PyObject*, PyObject*>* globlas) {_globals = globlas;}
};


#endif //PYTHONVM_FUNCTIONOBJECT_H
