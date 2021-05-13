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
    set_name(new PyString("cell"));
}

CellObject::CellObject(PyList *l, int i) :_index(i),_table(l) {
    set_kclass(CellKlass::getInstance());
}

size_t CellKlass::size() {
    return sizeof(CellKlass);
}

PyObject * CellObject::value() {
    return _table->get(_index);
}