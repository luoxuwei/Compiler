//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_STATEMENTEXECUTOR_H
#define C_COMPILER_STATEMENTEXECUTOR_H
#include "BaseExecutor.h"

class StatementExecutor : public BaseExecutor {
public:
    enum LoopType {
        FOR,
        WHILE,
        DO_WHILE
    };
    void * Execute(ICodeNode *root) override;
    bool isLoopContinute(ICodeNode *codeNode, LoopType type);
};


#endif //C_COMPILER_STATEMENTEXECUTOR_H
