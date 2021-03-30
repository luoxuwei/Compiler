//
// Created by Xuwei Luo 罗旭维 on 2021/3/29.
//

#include "FunctionObject.h"
#include "assert.h"
#include "universe.h"
#include "../object/PyDict.h"

FunctionKlass* FunctionKlass::_instance = NULL;

FunctionKlass::FunctionKlass() {

}

FunctionKlass * FunctionKlass::get_instance() {
    if (_instance == NULL) {
        //这里开始粗心写成_instance == new FunctionKlass();，
        //导致FunctionObject在调用klass()获取类型时，触发assert
        //调试了大半天才解决
        _instance = new FunctionKlass();
    }
    return _instance;
}

void FunctionKlass::print(PyObject *x) {
    FunctionObject *fo = static_cast<FunctionObject *>(x);
    assert(fo && fo->klass() == this);
    printf("<function: ");
    fo->func_name()->print();
    printf(">");
}

NativeFunctionClass* NativeFunctionClass::instance = NULL;
NativeFunctionClass::NativeFunctionClass() {

}
NativeFunctionClass * NativeFunctionClass::get_instance() {
    if (instance == NULL) {
        instance = new NativeFunctionClass();
    }
    return instance;
}

FunctionObject::FunctionObject(PyObject *code_object) {
    CodeObject* co = (CodeObject*) code_object;
    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;

    set_kclass(FunctionKlass::get_instance());
}

FunctionObject::FunctionObject(NativeFuncPointer nfp) {
    _func_code = NULL;
    _func_name = NULL;
    _flags = 0;
    _globals = NULL;
    _native_func = nfp;
    set_kclass(NativeFunctionClass::get_instance());
}

PyObject * FunctionObject::call(ArrayList<PyObject *> *args) {
    return (PyObject*) (*_native_func)(args);
}

void FunctionObject::set_defalts(ArrayList<PyObject *>* defaults) {
    if (defaults == NULL) {
        _defaults = NULL;
        return;
    }
    _defaults = new ArrayList<PyObject*>(defaults->length());
    for (int i=0; i<defaults->size(); i++) {
        _defaults->set(i, defaults->get(i));
    }
}

MethodKlass* MethodKlass::instance = NULL;
MethodKlass * MethodKlass::get_instance() {
    if (instance == NULL) {
        instance = new MethodKlass();
    }
    return instance;
}
MethodKlass::MethodKlass() {
    set_klass_dict(new PyDict());
}

bool MethodObject::is_function(PyObject *x) {
    return x->klass() == FunctionKlass::get_instance();
}



PyObject* len(ArrayList<PyObject*>* args) {
    return args->get(0)->len();
}

PyObject* string_upper(ArrayList<PyObject*>* args) {
    PyObject* arg0 = args->get(0);
    assert(arg0->klass() == StringKlass::get_instance());
    PyString* str_obj = (PyString*)arg0;

    int length = str_obj->length();
    if (length<=0) {
        return Universe::PyNone;
    }

    char* v = new char[length];
    char c;
    for (int i=0; i<length; i++) {
        c = str_obj->value()[i];
        if (c>='a' && c<='z') {
            v[i] = c-0x20;
        } else {
            v[i] = c;
        }
    }
    PyString* s = new PyString(v);
    delete v;
    return s;
}
