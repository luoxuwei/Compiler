//
// Created by 罗旭维 on 2021/10/24.
//

#ifndef C_COMPILER_OPTEXPREXECUTOR_H
#define C_COMPILER_OPTEXPREXECUTOR_H
#include "BaseExecutor.h"

class OptExprExecutor : public BaseExecutor {
public:
    void *Execute(ICodeNode *root);
};


#endif //C_COMPILER_OPTEXPREXECUTOR_H
