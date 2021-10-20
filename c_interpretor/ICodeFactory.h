//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_ICODEFACTORY_H
#define C_COMPILER_ICODEFACTORY_H
#include "ICodeNode.h"
#include "CTokenType.h"

class ICodeFactory {
public:
    static ICodeNode *createICodeNode(CTokenType::Token type);
};


#endif //C_COMPILER_ICODEFACTORY_H
