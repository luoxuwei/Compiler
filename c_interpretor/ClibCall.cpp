//
// Created by 罗旭维 on 2021/10/26.
//

#include "ClibCall.h"
#include "FunctionArgumentList.h"

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
}

Value * ClibCall::invokeAPI(string &name) {
    if (name == "printf") {
        return handlePrintfCall();
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