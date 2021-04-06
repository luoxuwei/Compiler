//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#ifndef PYTHONVM_KLASS_H
#define PYTHONVM_KLASS_H
class PyObject;
class PyString;
class PyDict;
/*
 * 需要一种机制来判断某个对象的实际类型到底是什么，在虚拟机中，最常用的解决办法就是Klass-Oop的二元结构。Klass代表一种具体类型
 * 它是类这个概念的实际体现。
 * 使用这种二元结构还有一个原因是，我们不希望在普通对象里引入虚函数机制，因为虚函数会在对象的开头引入虚表指针，二虚表指针会影响对象的属性在
 * 对象中的偏移。因此，就将类的方法定义和实现都放到Klass中，二在PyObject里只需要调用相应的Klass中的函数。
 *
 * */
class Klass {
private:
    PyString *_name;
    PyDict* _klass_dict;
public:
    Klass() {}

    static int compare_klass(Klass* x, Klass* y);

    void set_name(PyString *x) { _name = x; }

    PyString *name() { return _name; }

    void set_klass_dict(PyDict* d) { _klass_dict = d; }
    PyDict* klass_dict() { return _klass_dict; }

    virtual void print(PyObject* x){}

    virtual PyObject *greater(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *less(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *equal(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *not_equal(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *ge(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *le(PyObject *x, PyObject *y) { return 0; }



    virtual PyObject *add(PyObject *x, PyObject *y) {return 0;}

    virtual PyObject *sub(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *mul(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *div(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *mod(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *len(PyObject *x) {return 0;}
    virtual PyObject *subscr(PyObject* x, PyObject* y) {return 0;}
    virtual PyObject* contains(PyObject* x, PyObject* y) {return 0;}
    virtual void store_subscr(PyObject* x, PyObject* y, PyObject* z) {}
    virtual void delete_subscr(PyObject* x, PyObject* y) {};
    virtual PyObject* iter(PyObject* x) {};

};
#endif //PYTHONVM_KLASS_H
