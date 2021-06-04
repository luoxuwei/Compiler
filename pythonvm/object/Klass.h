//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#ifndef PYTHONVM_KLASS_H
#define PYTHONVM_KLASS_H
#include "../util/arraylist.h"

class PyTypeObject;
class PyObject;
class PyString;
class PyDict;
class PyList;
class PyTypeObject;
class OopClosure;
/*
 * 需要一种机制来判断某个对象的实际类型到底是什么，在虚拟机中，最常用的解决办法就是Klass-Oop的二元结构。Klass代表一种具体类型
 * 它是类这个概念的实际体现。
 * 使用这种二元结构还有一个原因是，我们不希望在普通对象里引入虚函数机制，因为虚函数会在对象的开头引入虚表指针，二虚表指针会影响对象的属性在
 * 对象中的偏移。因此，就将类的方法定义和实现都放到Klass中，二在PyObject里只需要调用相应的Klass中的函数。
 *
 * */
class Klass {
private:
    PyString *_name = NULL;
    PyDict* _klass_dict = NULL;
    PyTypeObject* _type_object = NULL;
    PyList* _super = NULL;
    PyList* _mro = NULL;//记录该类的方法解析顺序 method resolution order.
    //在python中类的定义一定会有先后顺序，所以绝对不可能出现两个类相互继承的情况。这样的话，无论继承的结构如何复杂，继承关系一定是
    //一个有向无环图。查找方法就是遍历这个图，遍历过程中发现某个类定义了就直接返回。python中是以深度优先遍历为基础，然后加了一点改进
    //区别在于遍历时被重复访问的节点，真正起作用的是最后一次。把深度优先遍历结果中重复的元素，只保留最后一次，就和实际探测的结果一致。
public:
    Klass();

    static int compare_klass(Klass* x, Klass* y);

    void add_super(Klass* k);

    PyTypeObject* super();

    void set_super_list(PyList* x) {_super = x;}

    PyList* mro() {return _mro;}

    void order_supers();

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



    virtual PyObject *add(PyObject *x, PyObject *y);

    virtual PyObject *sub(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *mul(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *div(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *mod(PyObject *x, PyObject *y) { return 0; }

    virtual PyObject *len(PyObject *x);
    virtual PyObject *subscr(PyObject* x, PyObject* y);
    virtual PyObject* contains(PyObject* x, PyObject* y) {return 0;}
    virtual void store_subscr(PyObject* x, PyObject* y, PyObject* z);
    virtual void delete_subscr(PyObject* x, PyObject* y) {};
    virtual PyObject* iter(PyObject* x) {};
    virtual PyObject* next(PyObject* x) {};
    void set_type_object(PyTypeObject* x) {_type_object = x;}
    PyTypeObject* type_object() {return _type_object;}
    virtual PyObject* allocate_instance(PyObject* type_object, ArrayList<PyObject*>* args);
    static PyObject* create_class(PyObject* x, PyObject* supers, PyObject* name);
    virtual PyObject* setattr(PyObject* x, PyObject* y, PyObject* z);
    virtual PyObject* getattr(PyObject* x, PyObject* y);
    PyObject* find_and_call(PyObject* lhs, ArrayList<PyObject*>* args, PyObject* func_name);
    PyObject* find_in_parents(PyObject* x, PyObject* y);
    virtual size_t size();
    virtual void oops_do(OopClosure* closure, PyObject* obj);
    virtual void oops_do(OopClosure* closure);
    void* operator new(size_t size);
};
#endif //PYTHONVM_KLASS_H
