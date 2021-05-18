//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "PyString.h"
#include "PyInteger.h"
#include <string.h>
#include <stdio.h>
#include "../runtime/universe.h"
#include "pytypeobject.h"

StringKlass* StringKlass::instance = NULL;

StringKlass * StringKlass::get_instance() {
    if (instance == NULL) {
        instance = new StringKlass();
    }
    return instance;
}

StringKlass::StringKlass() {
    //如果在这里调用set_name会和PyString陷入无限互调
    //PyString构造函数里调用StringKlass get_instance 初次调用创建StringKlass
    //StringKlass构造方法里调用set_name，要构造一个PyString，由回到PyString构造函数里
    //由于StringKlass构造还没完成，又会new StringKlass，在StringKlass构造方法里又构造PyString。。。
//    set_name(new PyString("str"));
}

void StringKlass::initialize() {
    (new PyTypeObject())->set_own_klass(this);
    set_name(new PyString("str"));
    add_super(ObjectKlass::get_instance());
}

void StringKlass::print(PyObject *x) {
    PyString* sx = (PyString*) x;
    assert(sx&&sx->klass() == (Klass* )this);

    for (int i=0; i<sx->length(); i++) {
        printf("%c", sx->value()[i]); //不能直接使用%s格式化输出，因为中间可能出现字符'\0'，所以只能逐字符。
    }
}

PyObject * StringKlass::subscr(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());
    PyString* sx = (PyString*) x;
    PyInteger* iy = (PyInteger*) y;
    return new PyString(&sx->value()[iy->value()], 1);
}

//坑！！，忘记实现PyString::equal就直接在map里用作key的类型，导致map put时一比较就返回true，第一个就返回了，永远只能改第一个。

PyObject * StringKlass::equal(PyObject *x, PyObject *y) {
    if (x->klass() != y->klass()) {
        return Universe::PyFalse;
    }

    PyString* sx = (PyString*) x;
    PyString* sy = (PyString*) y;

    assert(sx&&sx->klass() == (Klass* )this);
    assert(sy&&sy->klass() == (Klass* )this);

    if (sx->length() != sy->length()) {
        return Universe::PyFalse;
    }

    for (int i=0; i<sx->length(); i++) {
        if (sx->value()[i] != sy->value()[i]) {
            return Universe::PyFalse;
        }
    }
    return Universe::PyTrue;
}

PyObject * StringKlass::add(PyObject *x, PyObject *y) {
    PyString* sx = (PyString*) x;
    PyString* sy = (PyString*) y;

    assert(sx&&sx->klass() == (Klass* )this);
    assert(sy&&sy->klass() == (Klass* )this);

    int newLen = sx->length() + sy->length();
    char* temp = new char[newLen];
    for (int i = 0; i<sx->length(); i++) {
        temp[i] = sx->value()[i];
    }
    for (int i = sx->length()-1; i<(sx->length()+sy->length()); i++) {
        temp[i] = sy->value()[i - sx->length() + 1];
    }

    PyString* newStr = new PyString(temp, sx->length()+sy->length());
    delete[] temp;
    return newStr;
}

PyObject * StringKlass::len(PyObject *x) {
    assert( x&&x->klass() == this);
    return new PyInteger(((PyString* )x)->length());
}

PyObject * StringKlass::contains(PyObject *x, PyObject *y) {

}

PyObject * StringKlass::less(PyObject *x, PyObject *y) {
    assert(x && x->klass() == this);
    PyString* sx = (PyString*) x;

    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0) {
            return Universe::PyTrue;
        } else {
            return Universe::PyFalse;
        }
    }

    PyString* sy = (PyString*) y;
    assert(sy && sy->klass() == this);

    int len = sx->length() < sy->length()?sx->length():sy->length();
    for (int i=0; i<len; i++) {
        if (sx->value()[i] < sy->value()[i]) {
            return Universe::PyTrue;
        } else if (sx->value()[i] > sy->value()[i]) {
            return Universe::PyFalse;
        }
    }

    if (sx->length() < sy->length()) {
        return Universe::PyTrue;
    }

    return Universe::PyFalse;
}

PyObject * StringKlass::allocate_instance(PyObject* type_object, ArrayList<PyObject *> *args) {
    if (!args || args->length() == 0) {
        return new PyString("");
    } else {
        return NULL;
    }
}

PyString::PyString(const char *x) {
    _len = strlen(x);
    _value = new char[_len];
    strcpy(_value, x);
    set_kclass(StringKlass::get_instance());
}

PyString::PyString(const char *x, const int len) {
    _len = len;
    _value = new char[len];
    //do not use strcpy here, since '\0' is allowed.
    for (int i=0; i<len; i++) {
        _value[i] = x[i];
    }
    set_kclass(StringKlass::get_instance());
}







