//
// Created by Xuwei Luo 罗旭维 on 2021/5/25.
//

#ifndef PYTHONVM_OOPCLOSURE_H
#define PYTHONVM_OOPCLOSURE_H

class Space;
class Heap;

class PyObject;
class Klass;

template <typename T>
class ArrayList;

template <typename K, typename V>
class Map;

template <typename T>
class Stack;

class OopClosure {
public:
    virtual void do_oop(PyObject** obj) = 0;
    virtual void do_array_list(ArrayList<Klass* >** alist) = 0;
    virtual void do_array_list(ArrayList<PyObject*>** alist) = 0;
    virtual void do_map(Map<PyObject*, PyObject*>** amap) = 0;
    virtual void do_raw_mem(char** mem, int lenght) = 0;
    virtual void do_klass(Klass** k) = 0;
};
//垃圾回收非常适合访问者模式，OopClosure是访问者接口，针对不同的被访问者提供了具体的访问方法。例如对象是PyObject就用do_oop,map就用do_map
class ScavengeOopClosure: public OopClosure {
private:
    Space* _from;
    Space* _to;
    Space* _meta;

    Stack<PyObject*>* _oop_stack;
    PyObject* copy_and_push(PyObject* obj);

public:
    ScavengeOopClosure(Space* from, Space* to, Space* meta);
    virtual ~ScavengeOopClosure();
    virtual void do_oop(PyObject** obj);
    virtual void do_array_list(ArrayList<Klass* >** alist);
    virtual void do_array_list(ArrayList<PyObject*>** alist);
    virtual void do_map(Map<PyObject*, PyObject*>** amap);
    virtual void do_raw_mem(char** mem, int lenght);
    virtual void do_klass(Klass** k);

    void scavenge();
    void process_roots();
};


#endif //PYTHONVM_OOPCLOSURE_H
