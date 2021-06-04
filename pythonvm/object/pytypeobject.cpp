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
    add_super(ObjectKlass::get_instance());
    set_name(new PyString("type"));
}

void TypeKlass::print(PyObject *obj) {
    assert(obj->klass() == this);
    printf("<type ");
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
//A.field = "hello",A是类型不是对象
PyObject * TypeKlass::setattr(PyObject *x, PyObject *y, PyObject *z) {
    ((PyTypeObject*) x)->own_klass()->klass_dict()->put(y, z);
    return Universe::PyNone;
}

size_t TypeKlass::size() {
    return sizeof(PyTypeObject);
}

void PyTypeObject::set_own_klass(Klass *k) {
    _own_klass = k;
    k->set_type_object(this);
}

PyTypeObject::PyTypeObject() {
    set_kclass(TypeKlass::get_instance());
}

void TypeKlass::oops_do(OopClosure *closure, PyObject *obj) {

}