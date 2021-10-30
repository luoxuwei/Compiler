//
// Created by 罗旭维 on 2021/10/23.
//

#ifndef C_COMPILER_IFSTATEMENTEXECUTOR_H
#define C_COMPILER_IFSTATEMENTEXECUTOR_H
#include "BaseExecutor.h"


class IfStatementExecutor : public BaseExecutor {
public:
    void *Execute(ICodeNode *root);
};


#endif //C_COMPILER_IFSTATEMENTEXECUTOR_H
