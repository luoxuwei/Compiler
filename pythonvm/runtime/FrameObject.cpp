//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#include "FrameObject.h"

FrameObject::FrameObject() {

}

//这个构造函数是用于创建第一帧，这里没有新建一个变量表，而是直接
//用_locals，_locals和_globals指向同一个对象，这么做的原因是在非函数上下文中
//python的局部变量与全局变量的作用域是一样的，只有在调用函数时，创建了新的栈帧，才对局部变量和全局变量进行区分
FrameObject::FrameObject(CodeObject *code) {
    _consts = code->_consts;
    _names = code->_names;
    _locals = new Map<PyObject*, PyObject*>();
    _stack = new ArrayList<PyObject*>();
    _loop_stack = new ArrayList<Block*>();
    _globals = _locals; //
    _codes = code;
    _pc = 0;
    _sender = NULL;
}

FrameObject::FrameObject(FunctionObject *functionObject, ArrayList<PyObject*>* args) {

    _locals = new Map<PyObject*, PyObject*>();
    _stack = new ArrayList<PyObject*>();
    _loop_stack = new ArrayList<Block*>();
    _codes = functionObject->_func_code;
    _consts = _codes->_consts;
    _names = _codes->_names;
    _globals = functionObject->_globals;
    _pc = 0;
    _sender = NULL;
    if (args != NULL) {
        _fast_locals = new ArrayList<PyObject*>();
        for (int i=0; i<args->size(); i++) {
            _fast_locals->set(i, args->get(i));
        }
    }
}

int FrameObject::get_op_arg() {
    int byte1 = (_codes->_bytecodes->value()[_pc++] & 0xFF);
    return ((_codes->_bytecodes->value()[_pc++] & 0xFF) << 8) | byte1;
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}