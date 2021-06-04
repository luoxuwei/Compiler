//
// Created by Xuwei Luo 罗旭维 on 2021/5/25.
//

#include "oopClosure.h"
#include "../runtime/universe.h"
#include "../runtime/Interpreter.h"
#include "../runtime/StringTable.h"

void ScavengeOopClosure::scavenge() {
    //step1, mark roots
    process_roots();

    //step2, process all objects
    while (!_oop_stack->empty()) {
        _oop_stack->pop()->oops_do(this);
    }
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