//
// Created by 罗旭维 on 2021/10/26.
//

#include "ClibCall.h"
#include "FunctionArgumentList.h"
#include "MemoryHeap.h"

ClibCall *ClibCall::instance = NULL;

ClibCall * ClibCall::getInstance() {
    if (instance == NULL) {
        instance = new ClibCall();
    }
    return instance;
}

bool ClibCall::isAPICall(string &name) {
    auto iter = apiSet.find(name);
    if (iter != apiSet.end()) return true;
    return false;
}

ClibCall::ClibCall() {
    apiSet.insert("printf");
    apiSet.insert("malloc");
    apiSet.insert("sizeof");
}

Value * ClibCall::invokeAPI(string &name) {
    if (name == "printf") {
        return handlePrintfCall();
    } else if (name == "malloc") {
        return handleMallocCall();
    } else if (name == "sizeof") {
        return handleSizeOfCall();
    }
    return NULL;
}

Value * ClibCall::handlePrintfCall() {
    vector<Value *> *list = FunctionArgumentList::getInstance()->getFuncArgList(false);
    string *argStr = list->at(0)->u.s;
    string formatStr;
    int i = 0;
    int argCount = 1;
    while (i < argStr->size()) {
        if (argStr->at(i) == '%' && i+1 < argStr->size() && argStr->at(i+1) == 'd') {
            formatStr.append(list->at(argCount)->toString());
            argCount++;
            i = i+2;
        } else {
            formatStr.push_back(argStr->at(i));
            i++;
        }

    }
    printf(formatStr.c_str());
    printf("\n");
    return NULL;
}

Value * ClibCall::handleMallocCall() {
    vector<Value *> *list = FunctionArgumentList::getInstance()->getFuncArgList(false);
    int size = list->at(0)->u.i;
    int addr = 0;
    if (size > 0) {
        addr = MemoryHeap::allocMem(size);
    }
    return new Value(Value::POINTER, addr);
}

Value * ClibCall::handleSizeOfCall() {
    vector<Symbol *> *symList = FunctionArgumentList::getInstance()->getFuncArgSymbolList(false);
    return new Value(calculateVarSize(symList->at(0)));
}

int ClibCall::calculateVarSize(Symbol *s) {
    int size = 0;
    //arg list == NULL表示是基础数据类型
    if (s->getArgList() == NULL) {
        size = s->getByteSize();
    } else {
        Symbol *head = s->getArgList();
        while (head != NULL) {
            size += calculateVarSize(head);
            head = head->getNextSymbol();
        }
    }
    Declarator *declarator = s->getDeclarator(Declarator::ARRAY);
    if (declarator != NULL) {
        size = size * declarator->getElementNum();
    }
    return size;
}