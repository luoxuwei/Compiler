//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_BASEEXECUTOR_H
#define C_COMPILER_BASEEXECUTOR_H
#include "Executor.h"
#include "ICodeNode.h"

class BaseExecutor : public Executor {

public:
    void executeChildren(ICodeNode *root);
    ICodeNode * executeChild(ICodeNode *root, int childIdx);
    void copyChild(ICodeNode *root, ICodeNode *child);
};


#endif //C_COMPILER_BASEEXECUTOR_H
