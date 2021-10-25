//
// Created by 罗旭维 on 2021/10/25.
//

#include "CompoundStmtExecutor.h"


void * CompoundStmtExecutor::Execute(ICodeNode *root) {
    return executeChild(root, 0);
}