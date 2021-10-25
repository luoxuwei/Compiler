//
// Created by 罗旭维 on 2021/10/25.
//

#include "FunctionArgumentList.h"

FunctionArgumentList *FunctionArgumentList::instance = NULL;

FunctionArgumentList * FunctionArgumentList::getInstance() {
    if (instance == NULL) {
        instance = new FunctionArgumentList();
    }

    return instance;
}