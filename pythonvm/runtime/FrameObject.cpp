//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#include "FrameObject.h"

FrameObject::FrameObject() {

}

FrameObject::FrameObject(CodeObject *code) {
    _consts = code->_consts;
    _names = code->_names;
    _locals = new Map<PyObject*, PyObject*>();
    _stack = new ArrayList<PyObject*>();
    _loop_stack = new ArrayList<Block*>();
    _codes = code;
    _pc = 0;
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