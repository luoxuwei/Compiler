//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_UNARYNODEEXECUTOR_H
#define C_COMPILER_UNARYNODEEXECUTOR_H
#include "BaseExecutor.h"
#include "ICodeNode.h"
#include "Symbol.h"

class UnaryNodeExecutor : public BaseExecutor {
private:
    void setPointerValue(ICodeNode *root, Symbol *symbol, int index);
public:
    void * Execute(ICodeNode *root) override;
};


#endif //C_COMPILER_UNARYNODEEXECUTOR_H
