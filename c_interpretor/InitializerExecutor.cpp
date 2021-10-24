//
// Created by 罗旭维 on 2021/10/24.
//

#include "InitializerExecutor.h"


void * InitializerExecutor::Execute(ICodeNode *root) {
    //第0个孩子节点是Expr节点
    executeChild(root, 0);
    copyChild(root, root->getChildren()->at(0));
    return root;
}