//
// Created by 罗旭维 on 2021/10/25.
//

#include "FunctionArgumentList.h"
#include <algorithm>

FunctionArgumentList *FunctionArgumentList::instance = NULL;

FunctionArgumentList * FunctionArgumentList::getInstance() {
    if (instance == NULL) {
        instance = new FunctionArgumentList();
    }

    return instance;
}

//LRStateTablePase里，解析的时候，是往队头插入，所以最右的在最前面。在FunctDeclExecutor initArgumentList里设置参数时从Symbol里拿到的参数列表就是这种顺序
//但在执行时，都是先倒转子节点，与输入从左到右的顺序一致，然后执行的，所以这里保存的参数列表的值也是按准确的顺序排列的。
// 所以在initArgumentList时需要倒转，但在库函数执行时不需要,因为这里本身 就是正确的从顺序
vector<Value *> * FunctionArgumentList::getFuncArgList(bool reverse) {
    if (reverse) {
        std::reverse(funcArgList->begin(), funcArgList->end());
    }
    return funcArgList;
}

void FunctionArgumentList::setFuncArgSymbolList(vector<Symbol *> *list) {
    argSymList = list;
}

vector<Symbol *> * FunctionArgumentList::getFuncArgSymbolList(bool reverse) {
    if (reverse) {
        std::reverse(argSymList->begin(), argSymList->end());
    }
    return argSymList;
}