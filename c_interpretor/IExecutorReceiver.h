//
// Created by 罗旭维 on 2021/10/29.
//

#ifndef C_COMPILER_IEXECUTORRECEIVER_H
#define C_COMPILER_IEXECUTORRECEIVER_H
#include "ICodeNode.h"

class IExecutorReceiver {
public:
    virtual void handleExecutorMessage(ICodeNode *node) = 0;
};


#endif //C_COMPILER_IEXECUTORRECEIVER_H
