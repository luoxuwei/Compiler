//
// Created by 罗旭维 on 2021/10/25.
//

#ifndef C_COMPILER_ARGSEXECUTOR_H
#define C_COMPILER_ARGSEXECUTOR_H
#include "BaseExecutor.h"

class ArgsExecutor : public BaseExecutor {
public:
    void * Execute(ICodeNode *root) override;
};


#endif //C_COMPILER_ARGSEXECUTOR_H
