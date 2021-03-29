//
// Created by Xuwei Luo 罗旭维 on 2021/3/29.
//

#ifndef PYTHONVM_FUNCTIONOBJECT_H
#define PYTHONVM_FUNCTIONOBJECT_H
#include "../object/Klass.h"
#include "../code/CodeObject.h"


class FunctionKlass: public Klass {
private:
    static FunctionKlass* _instance;
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
};


#endif //PYTHONVM_FUNCTIONOBJECT_H
