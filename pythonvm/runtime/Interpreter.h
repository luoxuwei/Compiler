//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#ifndef PYTHONVM_INTERPRETER_H
#define PYTHONVM_INTERPRETER_H
#include "../util/arraylist.h"
#include "../object/PyObject.h"
#include "../code/CodeObject.h"
#include "../util/map.h"
class Interpreter {
private:
    ArrayList<PyObject*>* _stack;
    ArrayList<PyObject*>* _consts;
    Map<PyObject*, PyObject*>* _names;
public:
    void run(CodeObject* codeObject);
};


#endif //PYTHONVM_INTERPRETER_H
