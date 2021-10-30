//
// Created by 罗旭维 on 2021/10/23.
//

#ifndef C_COMPILER_TESTEXECUTOR_H
#define C_COMPILER_TESTEXECUTOR_H
#include "BaseExecutor.h"

class TestExecutor : public BaseExecutor {
    void *Execute(ICodeNode *root);
};


#endif //C_COMPILER_TESTEXECUTOR_H
