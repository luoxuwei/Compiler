//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_BASEEXECUTOR_H
#define C_COMPILER_BASEEXECUTOR_H
#include "Executor.h"
#include "ICodeNode.h"
#include "Value.h"

class BaseExecutor : public Executor {
private:
    static bool continueExecute;
    static Value *returnObj;

public:
    void executeChildren(ICodeNode *root);
    ICodeNode * executeChild(ICodeNode *root, int childIdx);
    void copyChild(ICodeNode *root, ICodeNode *child);
    void setReturnObj(Value *v) {returnObj = v;}
    Value *getReturnObj() {return returnObj;}
    void clearReturnObj() {returnObj = NULL;}
    void isContinueExecution(bool execute) {continueExecute = execute;}
};


#endif //C_COMPILER_BASEEXECUTOR_H
