//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_EXECUTORFACTORY_H
#define C_COMPILER_EXECUTORFACTORY_H
#include "Executor.h"

class ExecutorFactory {
private:
    static ExecutorFactory *instance;
    ExecutorFactory() {}
    ExecutorFactory(ExecutorFactory& executorFactory) {}

public:
    static ExecutorFactory *getInstance();
    Executor *getExecutor(ICodeNode *node);
};


#endif //C_COMPILER_EXECUTORFACTORY_H
