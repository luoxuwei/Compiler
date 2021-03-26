//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#ifndef PYTHONVM_INTERPRETER_H
#define PYTHONVM_INTERPRETER_H

#include "FrameObject.h"

class Interpreter {
private:
    FrameObject* _frame;
public:
    void run(CodeObject* codeObject);
};


#endif //PYTHONVM_INTERPRETER_H
