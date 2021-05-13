//
// Created by Xuwei Luo 罗旭维 on 2021/5/13.
//

#include "pytypeobject.h"
#include "PyDict.h"
#include "../runtime/StringTable.h"
#include "../runtime/universe.h"

TypeKlass* TypeKlass::_instance = NULL;

TypeKlass * TypeKlass::get_instance() {
    if (_instance == NULL) {
        _instance = new TypeKlass();
    }
    return _instance;
}

void TypeKlass::initialize() {
    (new PyTypeObject())->set_own_klass(this);
}

void TypeKlass::print(PyObject *obj) {
    assert(obj->klass() == this);
    printf("<type");
    Klass* own_klass = ((PyTypeObject*) obj)->own_klass();

    PyDict* attr_dict = own_klass->klass_dict();
    if (attr_dict) {
        PyObject* mod = attr_dict->get(StringTable::get_instance()->mod_str);
        if (mod != Universe::PyNone) {
            mod->print();
            printf(".");
        }
    }

    own_klass->name()->print();
    printf(">");
}

void PyTypeObject::set_own_klass(Klass *k) {
    _own_klass = k;
    k->set_type_object(this);
}

PyTypeObject::PyTypeObject() {
    set_kclass(TypeKlass::get_instance());
}