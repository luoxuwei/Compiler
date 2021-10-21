//
// Created by 罗旭维 on 2021/10/21.
//

#include "Intepretor.h"
#include "ExecutorFactory.h"

Intepretor * Intepretor::instance = NULL;

Intepretor * Intepretor::getInstance() {
    if (instance == NULL) {
        instance = new Intepretor();
    }

    return instance;
}

void * Intepretor::Execute(ICodeNode *root) {
    Executor *executor = ExecutorFactory::getInstance()->getExecutor(root);
    executor->Execute(root);
    return root;
}