//
// Created by 罗旭维 on 2021/10/24.
//

#include "EndOptExecutor.h"

void * EndOptExecutor::Execute(ICodeNode *root) {
    return executeChild(root, 0);
}