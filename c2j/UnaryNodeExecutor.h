//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_UNARYNODEEXECUTOR_H
#define C_COMPILER_UNARYNODEEXECUTOR_H
#include "BaseExecutor.h"
#include "ICodeNode.h"
#include "Symbol.h"
#include "IExecutorReceiver.h"
#include <stack>

class UnaryNodeExecutor : public BaseExecutor, IExecutorReceiver {
private:
    Symbol *structObjSymbol = NULL;
    Symbol *monitorSymbol = NULL;
    void setPointerValue(ICodeNode *root, Symbol *symbol, int index);
    bool isSymbolStructPointer(Symbol *symbol);
    void reverseStructSymbolList(Symbol *symbol, stack<Symbol *> &stack);
    int copyArrayVariableToMem(Symbol *symbol, char *buf, int offset);
    int writeStructVariablesToMem(Symbol *symbol, char *buf, int offset);//赋值时将新值拷贝到内存 myTag->v1 = 1
    int writeMemToStructVariables(Symbol *symbol, char *mem, int offset);//取值时将值从内存拷贝到结构体字段的symbol对象中 int v = myTag->v1
    int fromByteArrayToInteger(char *mem, int offset, int sz);
    int copyMemToArrayVariable(Symbol *symbol, char *mem, int offset);
    void copyBetweenStructAndMem(Symbol *symbol, bool isFromStructToMem);
    bool checkValidPointer(Symbol *symbol);
    void compileFunctionCall(string &funcName);
public:
    void * Execute(ICodeNode *root) override;
    void handleExecutorMessage(ICodeNode *node) override;
};


#endif //C_COMPILER_UNARYNODEEXECUTOR_H
