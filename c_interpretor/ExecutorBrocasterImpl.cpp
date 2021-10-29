//
// Created by 罗旭维 on 2021/10/29.
//

#include "ExecutorBrocasterImpl.h"

ExecutorBrocasterImpl *ExecutorBrocasterImpl::instance = NULL;

ExecutorBrocasterImpl * ExecutorBrocasterImpl::getInstance() {
    if (instance == NULL) {
        instance = new ExecutorBrocasterImpl();
    }
    return instance;
}

void ExecutorBrocasterImpl::brocastAfterExecution(ICodeNode *node) {
    notifyReceivers(afterExecutionReceiver, node);
}

void ExecutorBrocasterImpl::brocastBeforeExecution(ICodeNode *node) {
    notifyReceivers(beforeExecutionReceiver, node);
}

void ExecutorBrocasterImpl::registerReceiverForAfterExe(IExecutorReceiver *receiver) {
    auto iter = std::find(afterExecutionReceiver.begin(), afterExecutionReceiver.end(), receiver);
    if (iter == afterExecutionReceiver.end()) {
        afterExecutionReceiver.push_back(receiver);
    }
}

void ExecutorBrocasterImpl::registerReceiverForBeforeExe(IExecutorReceiver *receiver) {
    auto iter = std::find(beforeExecutionReceiver.begin(), beforeExecutionReceiver.end(), receiver);
    if (iter == beforeExecutionReceiver.end()) {
        beforeExecutionReceiver.push_back(receiver);
    }
}

void ExecutorBrocasterImpl::removeReceiver(IExecutorReceiver *receiver) {
    auto iter = std::find(beforeExecutionReceiver.begin(), beforeExecutionReceiver.end(), receiver);
    if (iter != beforeExecutionReceiver.end()) {
        std::remove(beforeExecutionReceiver.begin(), beforeExecutionReceiver.end(), receiver);
    }

    iter = std::find(afterExecutionReceiver.begin(), afterExecutionReceiver.end(), receiver);
    if (iter != afterExecutionReceiver.end()) {
        std::remove(afterExecutionReceiver.begin(), afterExecutionReceiver.end(), receiver);
    }

}

void ExecutorBrocasterImpl::notifyReceivers(vector<IExecutorReceiver *> &list, ICodeNode *node) {
    for (auto receiver : list) {
        receiver->handleExecutorMessage(node);
    }
}