//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#ifndef PYTHONVM_INTERPRETER_H
#define PYTHONVM_INTERPRETER_H

#include "FrameObject.h"
#include "../object/PyObject.h"

class Interpreter {
private:
    FrameObject* _frame;
    PyObject* _ret_value;
    Map<PyObject*, PyObject*>* _builtins;
public:
    Interpreter();
    void run(CodeObject* codeObject);
    void build_frame(PyObject* pyObjec, ArrayList<PyObject*>* argst);
    void destroy_frame();
    void leave_frame();
};


#endif //PYTHONVM_INTERPRETER_H
