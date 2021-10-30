//
// Created by 罗旭维 on 2021/10/25.
//

#include "ExtDefExecutor.h"
#include "GrammarInitializer.h"
#include "Value.h"
#include "TypeSystem.h"
#include "IValueSetter.h"
#include <string>

using namespace std;
void * ExtDefExecutor::Execute(ICodeNode *root) {
    this->root = root;
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    ICodeNode *child = NULL;
    Value *v = NULL;

    switch (production) {
        //需要执行的是FunctDec函数头和CompoundStmt函数体,OptSpecifiers不需要执行，它只是返回值的类型说明，相关的动作在语法解析时已经完成
        case GrammarInitializer::OptSpecifiers_FunctDecl_CompoundStmt_TO_ExtDef:

            child = root->getChildren()->at(0);
            funcName = (string *) child->getAttribute(ICodeNode::TEXT);
            root->setAttribute(ICodeNode::TEXT, funcName);

            saveArgs();//为应对递归调用的情况，在设置参数值之前保存环境（上一层执行的状态)
            //第一个节点就是NewName_LP_RP_TO_FunctDecl产生的FunctDecl
            executeChild(root, 0);

            //第二个节点是CompoundStmt对应的实际上是一个statement节点
            executeChild(root, 1);
            v = getReturnObj();
            clearReturnObj();
            if (v != NULL) {
                root->setAttribute(ICodeNode::VALUE, v);
            }
            isContinueExecution(true);//函数执行完之后恢复为true

            restoreArgs();
            break;
    }

    return root;
}

//由于所有的变量只在符号表里存有唯一的拷贝，所有对这个名字的改动都会有全局的影响
//所以每一次在函数调用前，先把它所有相关变量保存起来，再执行函数体里的语句执行完后再把原来保存的变量的信息设置回到变量中去
void ExtDefExecutor::saveArgs() {
    printf("\nSave arguments....\n");
    vector<Symbol *> args;
    TypeSystem::getInstance()->getSymbolsByScope(*funcName, args);
    int count = 0;
    while (count < args.size()) {
        Symbol *arg = args.at(count);
        Value *value = arg->getValue();
        argList.push_back(value);
        count++;
    }
}

void ExtDefExecutor::restoreArgs() {
    printf("\nRestore arguments....\n");
    vector<Symbol *> args;
    TypeSystem::getInstance()->getSymbolsByScope(*funcName, args);
    int count = 0;
    while (args.size() > 0 && count < argList.size()) {
        IValueSetter *setter = (IValueSetter *) args.at(count);

        Value *value = argList.at(count);
        setter->setValue(value);

        count++;
    }
}