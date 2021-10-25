//
// Created by 罗旭维 on 2021/10/25.
//

#ifndef C_COMPILER_FUNCTIONARGUMENTLIST_H
#define C_COMPILER_FUNCTIONARGUMENTLIST_H
#include "Value.h"
#include <vector>

class FunctionArgumentList {
private:
    static FunctionArgumentList *instance;
    vector<Value *> *funcArgList = NULL;
public:
    static FunctionArgumentList * getInstance();
    void setFuncArgList(vector<Value *> *list) {funcArgList = list;}
    vector<Value *> *getFuncArgList() {return funcArgList;}
};


#endif //C_COMPILER_FUNCTIONARGUMENTLIST_H
