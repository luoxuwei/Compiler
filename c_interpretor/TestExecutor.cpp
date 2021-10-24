//
// Created by 罗旭维 on 2021/10/23.
//

#include "TestExecutor.h"

void * TestExecutor::Execute(ICodeNode *root) {
    executeChildren(root);
    copyChild(root, root->getChildren()->at(0));
    return root;
}