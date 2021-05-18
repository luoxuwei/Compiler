//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#include "Klass.h"
#include "PyInteger.h"
#include "PyString.h"
#include "../runtime/universe.h"
#include "PyDict.h"
#include "PyList.h"
#include "pytypeobject.h"
#include "../runtime/FrameObject.h"
#include "../runtime/FunctionObject.h"

int Klass::compare_klass(Klass *x, Klass *y) {
    if (x == y) {
        return 0;
    }

    if (x == IntegerKlass::get_instance()) {
        return -1;
    } else if (y == IntegerKlass::get_instance()) {
        return 1;
    }

    if (x->name()->less(y->name()) == Universe::PyTrue) {
        return -1;
    } else {
        return 1;
    }

}

PyObject * Klass::create_class(PyObject *x, PyObject *supers, PyObject *name) {
    assert(x->klass() == DictKlass::get_instance());
    assert(supers->klass() == ListKlass::get_instance());
    assert(name->klass() == StringKlass::get_instance());
    Klass* new_klass = new Klass();
    PyDict* klass_dict = (PyDict*)x;
    PyList* supers_list = (PyList*)supers;
    new_klass->set_klass_dict(klass_dict);
    new_klass->set_name((PyString*) name);
    new_klass->set_super_list(supers_list);

    PyTypeObject* type_obj = new PyTypeObject();
    type_obj->set_own_klass(new_klass);
    return type_obj;
}

void Klass::add_super(Klass *k) {
    if (_super == NULL) {
        _super = new PyList();
    }
    _super->append(k->type_object());
}

PyTypeObject * Klass::super() {
    if (_super == NULL || _super->size() == 0) return NULL;

    return (PyTypeObject*)_super->get(0);
}

PyObject * Klass::allocate_instance(PyObject* type_object, ArrayList<PyObject *> *args) {
    PyObject* inst = new PyObject();
    inst->set_kclass(((PyTypeObject*) type_object)->own_klass());
    return inst;
}

PyObject * Klass::setattr(PyObject *x, PyObject *y, PyObject *z) {
    if (x->obj_dict() == NULL) {
        x->init_dict();
    }
    x->obj_dict()->put(y, z);
    return Universe::PyNone;
}

PyObject * Klass::getattr(PyObject *x, PyObject *y) {
    PyObject* result;
    if (x->obj_dict() != NULL) {
        result = x->obj_dict()->get(y);
        if (result != Universe::PyNone) return result;
    }

    result = klass_dict()->get(y);
    if (result == Universe::PyNone) return result;
//    if (MethodObject::is_function(result)) {
//    }
    if (result->klass() == FunctionKlass::get_instance()) {
        result = new MethodObject((FunctionObject*)result, x);
    }
    return result;
}