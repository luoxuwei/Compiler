//
// Created by 罗旭维 on 2021/10/29.
//

#ifndef C_COMPILER_EXECUTORBROCASTERIMPL_H
#define C_COMPILER_EXECUTORBROCASTERIMPL_H
#include "IExecutorBrocaster.h"
#include "IExecutorReceiver.h"
#include <vector>


class ExecutorBrocasterImpl : public IExecutorBrocaster {
private:
    vector<IExecutorReceiver *> beforeExecutionReceiver;
    vector<IExecutorReceiver *> afterExecutionReceiver;
    static ExecutorBrocasterImpl *instance;
    ExecutorBrocasterImpl() {};
    ExecutorBrocasterImpl(const ExecutorBrocasterImpl *) {};
    void notifyReceivers(vector<IExecutorReceiver *> &list, ICodeNode *node);
public:
    static ExecutorBrocasterImpl *getInstance();
    void brocastBeforeExecution(ICodeNode *node);
    void brocastAfterExecution(ICodeNode *node);
    void registerReceiverForBeforeExe(IExecutorReceiver *receiver);
    void registerReceiverForAfterExe(IExecutorReceiver *receiver);
    void removeReceiver(IExecutorReceiver *receiver);
};


#endif //C_COMPILER_EXECUTORBROCASTERIMPL_H
