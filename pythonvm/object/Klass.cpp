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
#include "../runtime/StringTable.h"
#include "../runtime/Interpreter.h"

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
    PyObject* init = inst->getattr(StringTable::get_instance()->init_str);
    if (init != Universe::PyNone) {
        Interpreter::get_instance()->call_virtual(init, args);
    }
    return inst;
}

PyObject * Klass::setattr(PyObject *x, PyObject *y, PyObject *z) {
    if (x->obj_dict() == NULL) {
        x->init_dict();
    }
    x->obj_dict()->put(y, z);
    return Universe::PyNone;
}
//bound和unbound function
//如果从klass的dict中找到的目标对象是一个函数的话，要将函数与调用者对象绑定在一起合成一个MethodObject
//如果是从obj dict中找到的函数就没有合成一个methodobject，在调用是要显示传入调用者对象
//a=A(1)
//b=A(2)
//def func(self, v)
//a.bar = func
//A.bar = func
//a.func(a, 1)
//b.func(1)
//如果一个函数没有绑定对象称之为unbound function;如果绑定了对象，它就是一个方法，称之为bound function。
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
    //必须加上NativeFuction，那些内置的str list等的method都是nativefunction必须包装成MethodObject，
    // 不然执行的时候就无法和调研对象绑定，build_frame.
    if (result->klass() == FunctionKlass::get_instance()
        || result->klass() == NativeFunctionClass::get_instance()) {
        result = new MethodObject((FunctionObject*)result, x);
    }
    return result;
}

PyObject * Klass::add(PyObject *x, PyObject *y) {
    ArrayList<PyObject*>* args = new ArrayList<PyObject*>();
    args->add(y);
    return find_and_call(x, args, StringTable::get_instance()->add_str);
}

PyObject * Klass::find_and_call(PyObject *lhs, ArrayList<PyObject *> *args, PyObject *func_name) {
    PyObject* func = lhs->getattr(func_name);
    if (func != Universe::PyNone) {
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    printf("class");
    lhs->klass()->name()->print();
    printf("Error: unsupport operation for class");
    assert(false);
    return Universe::PyNone;
}

PyObject * Klass::len(PyObject *x) {
    return find_and_call(x, NULL, StringTable::get_instance()->len_str);
}

