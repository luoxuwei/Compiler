//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#include "universe.h"
#include <stdio.h>
#include "../object/PyString.h"
#include "../object/PyDict.h"
#include "../runtime/FunctionObject.h"
#include "../object/PyList.h"
#include "../object/PyInteger.h"
#include "../object/pytypeobject.h"
#include "../runtime/cellObject.h"

PyObject* Universe::PyFalse = NULL;
PyObject* Universe::PyTrue = NULL;
PyObject* Universe::PyNone = NULL;
ArrayList<Klass*>* Universe::klasses   = NULL;
CodeObject* Universe::main_code = NULL;

void Universe::genesis() {
    PyFalse = new PyString("False");
    PyTrue = new PyString("True");
    PyNone = new PyString("None");
    klasses = new ArrayList<Klass*>();


    Klass* object_klass = ObjectKlass::get_instance();
    Klass* type_klass   = TypeKlass::get_instance();

    PyTypeObject* tp_obj = new PyTypeObject();
    tp_obj->set_own_klass(type_klass);

    PyTypeObject* obj_obj = new PyTypeObject();
    obj_obj->set_own_klass(object_klass);

    type_klass->add_super(object_klass);
    // do nothing for object klass
    //object_klass->add_super(NULL);

    ObjectKlass::get_instance()->initialize();

    PyDict* klass_dict = new PyDict();
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    klass_dict->put(new PyString("upper"), new FunctionObject(string_upper));

    IntegerKlass::get_instance()->initialize();
    DictKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();
    TypeKlass::get_instance()->initialize();

    type_klass->set_klass_dict(new PyDict());
    object_klass->set_klass_dict(new PyDict());

    type_klass->set_name(new PyString("type"));
    object_klass->set_name(new PyString("object"));

    IntegerKlass::get_instance()->order_supers();
    DictKlass::get_instance()->order_supers();
    StringKlass::get_instance()->order_supers();
    ListKlass::get_instance()->order_supers();
    TypeKlass::get_instance()->order_supers();
    FunctionKlass::get_instance()->order_supers();
    NativeFunctionClass::get_instance()->order_supers();
    CellKlass::getInstance()->order_supers();
}

void Universe::destroy() {

}

void Universe::oops_do(OopClosure *closure) {
    closure->do_oop(&PyTrue);
    closure->do_oop(&PyFalse);
    closure->do_oop(&PyNone);
    closure->do_oop((PyObject**)&main_code);
    closure->do_array_list(&klasses);
}