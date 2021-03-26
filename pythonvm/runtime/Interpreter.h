//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#ifndef PYTHONVM_INTERPRETER_H
#define PYTHONVM_INTERPRETER_H
#include "../util/arraylist.h"
#include "../object/PyObject.h"
#include "../code/CodeObject.h"
#include "../util/map.h"

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

class Interpreter {
private:
    ArrayList<PyObject*>* _stack;
    ArrayList<PyObject*>* _consts;
    ArrayList<Block*>* _loop_stack;
    Map<PyObject*, PyObject*>* _names;
public:
    void run(CodeObject* codeObject);
};


#endif //PYTHONVM_INTERPRETER_H
