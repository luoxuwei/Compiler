//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#ifndef PYTHONVM_FRAMEOBJECT_H
#define PYTHONVM_FRAMEOBJECT_H
#include "../code/CodeObject.h"
#include "../object/PyObject.h"
#include "../util/arraylist.h"
#include "../object/PyObject.h"
#include "../code/CodeObject.h"
#include "../util/map.h"
#include "FunctionObject.h"

class Block {
public:
    unsigned char _type;
    unsigned int _target;
    int _level;

    Block() {
        _type = 0;
        _target = 0;
        _level = 0;
    }

    Block(unsigned char b_type, unsigned int b_target, int b_level):_type(b_type),_target(b_target),_level(b_level) {}

    Block(const Block& b) {
        _type = b._type;
        _target = b._target;
        _level = b._level;
    }
};

class FrameObject {
public:
    FrameObject(CodeObject* code);
    FrameObject(FunctionObject* functionObject, ArrayList<PyObject*>* args);
    FrameObject();
    ArrayList<PyObject*>* _stack;
    ArrayList<PyObject*>* _consts;
    ArrayList<PyObject*>* _names;
    ArrayList<Block*>* _loop_stack;
    Map<PyObject*, PyObject*>* _locals;
    Map<PyObject*, PyObject*>* _globals;
    ArrayList<PyObject*>* _fast_locals;

    CodeObject* _codes;
    int _pc;
    FrameObject* _sender;

public:
    void set_pc(int pc) {_pc=pc;}
    int get_pc() {return _pc;}
    FrameObject* get_sender() {return _sender;}
    void set_sender(FrameObject* sender) {
        _sender = sender;
    }

    ArrayList<PyObject*>* stack() {return _stack;}
    ArrayList<PyObject*>* consts() {return _consts;}
    ArrayList<Block*>* loop_stack() {return _loop_stack;}
    ArrayList<PyObject*>* names() {return _names;}
    Map<PyObject*, PyObject*>* locals() {return _locals;}
    Map<PyObject*, PyObject*>* globals() {return _globals;}
    ArrayList<PyObject*>* fast_locals() {return _fast_locals;}

    bool has_more_codes();
    unsigned char get_op_code();
    int get_op_arg();
    bool is_first_frame() {return _sender == NULL;}
};


#endif //PYTHONVM_FRAMEOBJECT_H
