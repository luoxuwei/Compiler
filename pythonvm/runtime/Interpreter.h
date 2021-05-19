//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#ifndef PYTHONVM_INTERPRETER_H
#define PYTHONVM_INTERPRETER_H

#include "FrameObject.h"
#include "../object/PyObject.h"

class Interpreter {
private:
    FrameObject* _frame = NULL;
    PyObject* _ret_value = NULL;
    Map<PyObject*, PyObject*>* _builtins = NULL;
    Interpreter();
    static Interpreter* _instance;
public:
    static Interpreter* get_instance();
    void run(CodeObject* codeObject);
    void build_frame(PyObject* pyObjec, ArrayList<PyObject*>* argst, int op_arg);
    void destroy_frame();
    void leave_frame();
    void eval_frame();
    PyObject* call_virtual(PyObject* func, ArrayList<PyObject*>* args);
    void enter_frame(FrameObject* frame);
};


#endif //PYTHONVM_INTERPRETER_H
