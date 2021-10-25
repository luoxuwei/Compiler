//
// Created by 罗旭维 on 2021/10/25.
//

#include "FunctDeclExecutor.h"
#include "GrammarInitializer.h"

void * FunctDeclExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    switch (production) {
        case GrammarInitializer::NewName_LP_RP_TO_FunctDecl:
            //没有参数传递，只是把下一个节点的内容拷贝到当前节点上来，下一个节点就是NAME_TO_NewName产生的NewName节点，只是记录了变量名的字符串
            root->reverseChildren();
            copyChild(root, root->getChildren()->at(0));
            break;
    }

    return root;
}