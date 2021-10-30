//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_EXECUTOR_H
#define C_COMPILER_EXECUTOR_H
#include "ICodeNode.h"

class Executor {
public:
    virtual void *Execute(ICodeNode *root) = 0;
};


#endif //C_COMPILER_EXECUTOR_H
