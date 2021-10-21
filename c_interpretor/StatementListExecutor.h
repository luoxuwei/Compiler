//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_STATEMENTLISTEXECUTOR_H
#define C_COMPILER_STATEMENTLISTEXECUTOR_H
#include "BaseExecutor.h"


class StatementListExecutor : public BaseExecutor {
public:
    void * Execute(ICodeNode *root) override;
};


#endif //C_COMPILER_STATEMENTLISTEXECUTOR_H
