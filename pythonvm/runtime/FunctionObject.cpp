//
// Created by Xuwei Luo 罗旭维 on 2021/3/29.
//

#include "FunctionObject.h"
#include "assert.h"

FunctionKlass* FunctionKlass::_instance = NULL;

FunctionKlass::FunctionKlass() {

}

FunctionKlass * FunctionKlass::get_instance() {
    if (_instance == NULL) {
        _instance == new FunctionKlass();
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

FunctionObject::FunctionObject(PyObject *code_object) {
    CodeObject* co = (CodeObject*) code_object;
    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;

    set_kclass(FunctionKlass::get_instance());
}
