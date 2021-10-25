//
// Created by 罗旭维 on 2021/10/25.
//

#ifndef C_COMPILER_EXTDEFEXECUTOR_H
#define C_COMPILER_EXTDEFEXECUTOR_H
#include "BaseExecutor.h"

class ExtDefExecutor : public BaseExecutor {
public:
    void * Execute(ICodeNode *root);
};


#endif //C_COMPILER_EXTDEFEXECUTOR_H
