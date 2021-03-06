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
#include "../util/arraylist.h"
#include "../runtime/universe.h"
#include "../memory/heap.h"

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
    new_klass->order_supers();
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
    PyObject* func = x->klass()->klass_dict()->get(StringTable::get_instance()->setattr_str);
    if (func != Universe::PyNone && (func->klass() == FunctionKlass::get_instance()
            || func->klass() == NativeFunctionClass::get_instance())) {
        func = new MethodObject((FunctionObject*) func, x);
        ArrayList<PyObject*>* args = new ArrayList<PyObject*>();
        args->add(y);
        args->add(z);
        return Interpreter::get_instance()->call_virtual(func, args);
    }
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
    PyObject* func = find_in_parents(x, StringTable::get_instance()->getattr_str);
    if (func != Universe::PyNone && (func->klass() == FunctionKlass::get_instance()
                                     || func->klass() == NativeFunctionClass::get_instance())) {
        func = new MethodObject((FunctionObject*) func, x);
        ArrayList<PyObject*>* args = new ArrayList<PyObject*>();
        args->add(y);
        return Interpreter::get_instance()->call_virtual(func, args);
    }
    PyObject* result = Universe::PyNone;
    if (x->obj_dict() != NULL) {
        result = x->obj_dict()->get(y);
        if (result != Universe::PyNone) return result;
    }

    result = find_in_parents(x, y);
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

//处理继承
PyObject * Klass::find_in_parents(PyObject *x, PyObject *y) {
    PyObject* result = Universe::PyNone;
    result = x->klass()->klass_dict()->get(y);
    if(result != Universe::PyNone) return result;

    //find attribute in all parents
    if (x->klass()->mro() == NULL)
        return result;

    for (int i=0; i<x->klass()->mro()->size(); i++) {
        result = ((PyTypeObject*)x->klass()->mro()->get(i))->own_klass()->klass_dict()->get(y);
        if (result != Universe::PyNone) return result;
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

PyObject * Klass::subscr(PyObject *x, PyObject *y) {
    ArrayList<PyObject*>* args = new ArrayList<PyObject*>();
    args->add(y);
    return find_and_call(x, args, StringTable::get_instance()->getitem_str);
}

void Klass::store_subscr(PyObject *x, PyObject *y, PyObject *z) {
    ArrayList<PyObject*>* args = new ArrayList<PyObject*>();
    args->add(y);
    args->add(z);
    find_and_call(x, args, StringTable::get_instance()->setitem_str);
}

void Klass::order_supers() {
    if (_super == NULL) return;

    if (_mro == NULL) {
        _mro = new PyList();
    }

    int cur = -1;
    //cur用来判断两个父类中的mro顺序出现冲突的情况。考虑下面的例子：
    // class A(X,Y), class B(Y,X), class C(B,A) python会报错
    //因为B的mro是XY，A的mro是YX，当A和B做C的父类时，C的mro就出现矛盾。python会抛异常
    //这种冲突的前提条件是两个元素在两个直接父类的mro中都出现并且在不同父类里的顺序不同。
    //使用cur记录上一个元素出现重复的情况，如果在不同父类里的顺序不同，则直接报错退出
    for (int i=0; i<_super->size(); i++) {
        PyTypeObject* tp_obj = (PyTypeObject*)_super->get(i);
        _mro->append(tp_obj);
        if (tp_obj->own_klass()->mro() == NULL) {
            continue;
        }
        Klass* k = tp_obj->own_klass();
        for (int j=0; j<k->mro()->size(); j++) {
            int index = _mro->inner_list()->index(k->mro()->get(j));
            if (index < cur) {
                printf("Error: method resolution order conflicts.\n");
                assert(false);
            }
            cur = index;
            if (index > 0) {
                _mro->inner_list()->delete_index(index);
            }
            _mro->append(k->mro()->get(j));
        }


    }

    if (_mro == NULL) {
        return;
    }

    printf("%s' s mro is ", _name->value());
    for (int i=0; i<_mro->size(); i++) {
        PyTypeObject* tp_obj = (PyTypeObject*) (_mro->get(i));
        Klass* k = tp_obj->own_klass();
        printf("%s, ", k->name()->value());
    }
    printf("\n");
}

size_t Klass::size() {
    return sizeof(PyObject);
}

Klass::Klass() {
    Universe::klasses->add(this);
}

void Klass::oops_do(OopClosure *closure, PyObject *obj) {
    printf("warning: klass oops_do for ");
    _name->print();
    printf("\n");
}

void Klass::oops_do(OopClosure* f) {
    f->do_oop((PyObject**)&_super);
    f->do_oop((PyObject**)&_mro);
    f->do_oop((PyObject**)&_name);
    f->do_oop((PyObject**)&_klass_dict);
    f->do_oop((PyObject**)&_type_object);
}

void * Klass::operator new(size_t size) {
    return Universe::heap->allocate_meta(size);
}