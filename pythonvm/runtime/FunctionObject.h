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
    FunctionKlass();

public:
    static FunctionKlass* get_instance();
    virtual void print(PyObject* x) override;
};

typedef PyObject* (*NativeFuncPointer)(ArrayList<PyObject*>* args);

class FunctionObject: public PyObject {
    friend class FunctionKlass;
    friend class FrameObject;
private:
    CodeObject* _func_code;
    PyString* _func_name;
    unsigned int _flags;
    //函数所依赖的全局变量是定义函数对象的时候，而不是调用函数的时候。这就要求必须为FunctionObject对象引入一个global变量表。
    Map<PyObject*, PyObject*>* _globals;
    ArrayList<PyObject*>* _defaults;
    NativeFuncPointer _native_func;
public:
    FunctionObject(PyObject* code_object);
    FunctionObject(Klass* klass) {
        _func_code = NULL;
        _func_name = NULL;
        _flags = 0;
        _defaults = NULL;
        set_kclass(klass);
    }

    FunctionObject(NativeFuncPointer nfp);

    PyString* func_name() {return _func_name;}
    unsigned int flags() {return _flags;}
    Map<PyObject*, PyObject*>* globals() {return _globals;}
    void set_gloabls(Map<PyObject*, PyObject*>* globlas) {_globals = globlas;}
    void set_defalts(ArrayList<PyObject*>* defaults);
    ArrayList<PyObject*>* defaults() {return _defaults;}
    PyObject* call(ArrayList<PyObject*>* args);
};

class NativeFunctionClass: public Klass {
private:
    static NativeFunctionClass* instance;
    NativeFunctionClass();

public:
    static NativeFunctionClass* get_instance();
};

class MethodKlass: public Klass {
private:
    static MethodKlass* instance;
    MethodKlass();

public:
    static MethodKlass* get_instance();
};

class MethodObject: public PyObject {
private:
    PyObject* _owner;
    FunctionObject* _func;
public:
    MethodObject(FunctionObject* functionObject):_owner(NULL),_func(functionObject){
        set_kclass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject* functionObject, PyObject* owner):_owner(owner),_func(functionObject){
        set_kclass(MethodKlass::get_instance());
    }

    void set_owner(PyObject* owner) {_owner = owner;}
    PyObject* owner() {return _owner;}
    FunctionObject* func() {return _func;}
    static bool is_function(PyObject* x);
};

PyObject* len(ArrayList<PyObject*>* args);
PyObject* string_upper(ArrayList<PyObject*>* args);
PyObject* list_append(ArrayList<PyObject*>* args);


#endif //PYTHONVM_FUNCTIONOBJECT_H
