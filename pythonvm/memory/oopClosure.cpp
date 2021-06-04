//
// Created by Xuwei Luo 罗旭维 on 2021/5/25.
//

#include "oopClosure.h"
#include "../runtime/universe.h"
#include "../runtime/Interpreter.h"
#include "../runtime/StringTable.h"
#include "../util/stack.h"
#include "heap.h"

void ScavengeOopClosure::scavenge() {
    //step1, mark roots
    process_roots();

    //step2, process all objects
    while (!_oop_stack->empty()) {
        _oop_stack->pop()->oops_do(this);
    }
}

ScavengeOopClosure::ScavengeOopClosure(Space *from, Space *to, Space *meta) {
    _from = from;
    _to = to;
    _meta = meta;
    _oop_stack = new Stack<PyObject*>(1024);
}

ScavengeOopClosure::~ScavengeOopClosure() {
    _from = NULL;
    _to = NULL;

    delete _oop_stack;
    _oop_stack = NULL;
}

void ScavengeOopClosure::process_roots() {
    Universe::oops_do(this);
    Interpreter::get_instance()->oops_do(this);
    StringTable::get_instance()->oops_do(this);
}

void ScavengeOopClosure::do_oop(PyObject** obj) {
    if (obj == NULL || *obj == NULL) return;

    if (!_from->has_obj((char*)*obj)) {
        return;
    }
    (*obj) = copy_and_push(*obj);
}

PyObject * ScavengeOopClosure::copy_and_push(PyObject *obj) {
    char* target = obj->new_address();
    if (target) {
        return (PyObject*)target;
    }

    //copy
    int size = obj->size();
    target = (char*)_to->allocate(size);
    memcpy(target, obj, size);
    obj->set_new_address(target);

    //push
    _oop_stack->push((PyObject *)target);
    return (PyObject*)target;
}

void ScavengeOopClosure::do_raw_mem(char **mem, int lenght) {
    if (*mem == NULL) {
        return;
    }

    char* target = (char*) _to->allocate(lenght);
    memcpy(target, (*mem), lenght);
    (*mem) = target;
}

void ScavengeOopClosure::do_array_list(ArrayList<Klass *> **alist) {
    if (alist == NULL || *alist == NULL) {
        return;
    }

    assert(_from->has_obj((char *)(*alist)));//防止重复回收
    size_t size = sizeof(ArrayList<PyObject*>);
    char *target = (char*)_to->allocate(size);
    memcpy(target, *alist, size);
    *(char **)alist = target;
    (*alist)->oops_do(this);
}

//两个do_array_list 方法逻辑一样，不同的仅仅是输入的参数类型，正好可以用模版，但c++中虚方法不可以是模版方法所以只能用重载
void ScavengeOopClosure::do_array_list(ArrayList<PyObject *> **alist) {
    if (alist == NULL || *alist == NULL)
        return;

    assert(_from->has_obj((char*)*alist));

    size_t size = sizeof(ArrayList<PyObject*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, (*alist), size);
    (*(char**)alist) = target;
    (*alist)->oops_do(this);
}

//class对象都放在meta空间不需要移动
void ScavengeOopClosure::do_klass(Klass **k) {
    if (k == NULL || *k == NULL)
        return;

    (*k)->oops_do(this);
}

void ScavengeOopClosure::do_map(Map<PyObject *, PyObject *> **amap) {
    if (amap == NULL || *amap == NULL) {
        return;
    }

    assert(_from->has_obj((char *)*amap));

    size_t size = sizeof(Map<PyObject*, PyObject*>);
    char* target = (char *)_to->allocate(size);
    memcpy(target, *amap, size);
    *(char **)amap = target;
    (*amap)->oops_do(this);
}