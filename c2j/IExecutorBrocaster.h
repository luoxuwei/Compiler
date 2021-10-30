//
// Created by 罗旭维 on 2021/10/29.
//

#ifndef C_COMPILER_IEXECUTORBROCASTER_H
#define C_COMPILER_IEXECUTORBROCASTER_H
#include "ICodeNode.h"
#include "IExecutorReceiver.h"

class IExecutorBrocaster {
public:
    virtual void brocastBeforeExecution(ICodeNode *node) = 0;
    virtual void brocastAfterExecution(ICodeNode *node) = 0;
    virtual void registerReceiverForBeforeExe(IExecutorReceiver *receiver) = 0;
    virtual void registerReceiverForAfterExe(IExecutorReceiver *receiver) = 0;
    virtual void removeReceiver(IExecutorReceiver *receiver) = 0;
};


#endif //C_COMPILER_IEXECUTORBROCASTER_H
