//
// Created by 罗旭维 on 2021/10/21.
//

#include "StatementListExecutor.h"

void * StatementListExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    return root;
}