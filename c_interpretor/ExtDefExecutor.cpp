//
// Created by 罗旭维 on 2021/10/25.
//

#include "ExtDefExecutor.h"
#include "GrammarInitializer.h"
#include "Value.h"
#include <string>

using namespace std;
void * ExtDefExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    ICodeNode *child = NULL;
    string *funcName = NULL;
    Value *v = NULL;

    switch (production) {
        //需要执行的是FunctDec函数头和CompoundStmt函数体,OptSpecifiers不需要执行，它只是返回值的类型说明，相关的动作在语法解析时已经完成
        case GrammarInitializer::OptSpecifiers_FunctDecl_CompoundStmt_TO_ExtDef:
            //第一个节点就是NewName_LP_RP_TO_FunctDecl产生的FunctDecl
            executeChild(root, 0);
            child = root->getChildren()->at(0);
            funcName = (string *) child->getAttribute(ICodeNode::TEXT);
            root->setAttribute(ICodeNode::TEXT, funcName);

            //第二个节点是CompoundStmt对应的实际上是一个statement节点
            executeChild(root, 1);
            v = getReturnObj();
            clearReturnObj();
            if (v != NULL) {
                root->setAttribute(ICodeNode::VALUE, v);
            }
            isContinueExecution(true);//函数执行完之后恢复为true
            break;
    }

    return root;
}