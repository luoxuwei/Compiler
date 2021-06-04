//
// Created by Xuwei Luo 罗旭维 on 2021/3/29.
//

#ifndef PYTHONVM_FUNCTIONOBJECT_H
#define PYTHONVM_FUNCTIONOBJECT_H
#include "../object/Klass.h"
#include "../code/CodeObject.h"
#include "../util/map.h"
#include "../object/PyList.h"

class FunctionKlass: public Klass {
private:
    static FunctionKlass* _instance;
    FunctionKlass();

public:
    static FunctionKlass* get_instance();
    virtual void print(PyObject* x) override;
    size_t size() override;
    void oops_do(OopClosure *closure, PyObject *obj) override;
};

typedef PyObject* (*NativeFuncPointer)(ArrayList<PyObject*>* args);

class FunctionObject: public PyObject {
    friend class FunctionKlass;
    friend class FrameObject;
    friend class NativeFunctionClass;
private:
    CodeObject* _func_code = NULL;
    PyString* _func_name = NULL;
    unsigned int _flags;
    //函数所依赖的全局变量是定义函数对象的时候，而不是调用函数的时候。这就要求必须为FunctionObject对象引入一个global变量表。
    PyDict* _globals = NULL;
    ArrayList<PyObject*>* _defaults = NULL;
    NativeFuncPointer _native_func = NULL;
    PyList* _closure = NULL;
public:
    enum CO_FLAGS {
        CO_VARARGS = 0x4,
        CO_VARKEYWORDS = 0x8,
        CO_GENERATOR = 0x20,
    };

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
    PyDict* globals() {return _globals;}
    void set_gloabls(PyDict* globlas) {_globals = globlas;}
    void set_defalts(ArrayList<PyObject*>* defaults);
    ArrayList<PyObject*>* defaults() {return _defaults;}
    PyObject* call(ArrayList<PyObject*>* args);
    PyList* closure() {return _closure;}
    void set_closure(PyList* x) {_closure = x;}
};

class NativeFunctionClass: public Klass {
private:
    static NativeFunctionClass* instance;
    NativeFunctionClass();

public:
    static NativeFunctionClass* get_instance();
    size_t size() override;
    void oops_do(OopClosure *closure, PyObject *obj) override;
};

class MethodKlass: public Klass {
private:
    static MethodKlass* instance;
    MethodKlass();

public:
    static MethodKlass* get_instance();
    size_t size() override;
    void oops_do(OopClosure *closure, PyObject *obj) override;
};

class MethodObject: public PyObject {
    friend class MethodKlass;
private:
    PyObject* _owner = NULL;
    FunctionObject* _func = NULL;
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
    static bool is_method(PyObject* x);
};

PyObject* len(ArrayList<PyObject*>* args);
PyObject* string_upper(ArrayList<PyObject*>* args);
PyObject* list_append(ArrayList<PyObject*>* args);
PyObject* list_index(ArrayList<PyObject*>* args);
PyObject* list_pop(ArrayList<PyObject*>* args);
PyObject* list_remove(ArrayList<PyObject*>* args);
PyObject* list_reverse(ArrayList<PyObject*>* args);
PyObject* list_sort(ArrayList<PyObject*>* args);
PyObject* isinstance(ArrayList<PyObject*>* args);
PyObject* type_of(ArrayList<PyObject*>* args);
#endif //PYTHONVM_FUNCTIONOBJECT_H
