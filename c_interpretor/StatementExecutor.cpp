//
// Created by 罗旭维 on 2021/10/21.
//

#include "StatementExecutor.h"

void * StatementExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    return root;
}