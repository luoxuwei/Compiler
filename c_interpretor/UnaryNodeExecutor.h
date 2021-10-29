//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_UNARYNODEEXECUTOR_H
#define C_COMPILER_UNARYNODEEXECUTOR_H
#include "BaseExecutor.h"
#include "ICodeNode.h"
#include "Symbol.h"
#include "IExecutorReceiver.h"

class UnaryNodeExecutor : public BaseExecutor, IExecutorReceiver {
private:
    Symbol *structObjSymbol = NULL;
    Symbol *monitorSymbol = NULL;
    void setPointerValue(ICodeNode *root, Symbol *symbol, int index);
    bool isSymbolStructPointer(Symbol *symbol);
public:
    void * Execute(ICodeNode *root) override;
    void handleExecutorMessage(ICodeNode *node) override;
};


#endif //C_COMPILER_UNARYNODEEXECUTOR_H
