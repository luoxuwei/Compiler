//
// Created by 罗旭维 on 2021/10/25.
//

#ifndef C_COMPILER_EXTDEFEXECUTOR_H
#define C_COMPILER_EXTDEFEXECUTOR_H
#include "BaseExecutor.h"
#include "Value.h"
#include "ICodeNode.h"

class ExtDefExecutor : public BaseExecutor {
private:
    vector<Value *> argList;
    ICodeNode *root;
    string *funcName;
    void saveArgs();
    void restoreArgs();
public:
    void * Execute(ICodeNode *root);
};


#endif //C_COMPILER_EXTDEFEXECUTOR_H
