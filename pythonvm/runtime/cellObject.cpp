//
// Created by Xuwei Luo 罗旭维 on 2021/5/13.
//

#include "cellObject.h"
#include "../object/PyDict.h"
#include "../object/PyString.h"
#include "../object/pytypeobject.h"

CellKlass* CellKlass::_instance = NULL;
CellKlass * CellKlass::getInstance() {
    if (_instance == NULL) {
        _instance = new CellKlass();
    }
    return _instance;
}

CellKlass::CellKlass() {
    set_klass_dict(new PyDict());
    (new PyTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
    set_name(new PyString("cell"));
}

CellObject::CellObject(PyList *l, int i) :_index(i),_table(l) {
    set_kclass(CellKlass::getInstance());
}

size_t CellKlass::size() {
    return sizeof(CellObject);
}

PyObject * CellObject::value() {
    return _table->get(_index);
}

void CellKlass::oops_do(OopClosure *closure, PyObject *obj) {
    CellObject* co = (CellObject*) obj;
    closure->do_oop((PyObject**)&co->_table);
}