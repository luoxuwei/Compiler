//
// Created by 罗旭维 on 2021/10/25.
//

#ifndef C_COMPILER_FUNCTDECLEXECUTOR_H
#define C_COMPILER_FUNCTDECLEXECUTOR_H
#include "BaseExecutor.h"
#include <vector>
#include "TypeSystem.h"
#include "Symbol.h"
#include "Value.h"

class FunctDeclExecutor : public BaseExecutor {
private:
    ICodeNode *currentNode;
    vector<Value *> *argList;
    void initArgumentList(Symbol *args);

public:
    void * Execute(ICodeNode *root);
    void removeArgsFromTable();

};


#endif //C_COMPILER_FUNCTDECLEXECUTOR_H
