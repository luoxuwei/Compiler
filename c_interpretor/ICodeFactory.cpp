//
// Created by 罗旭维 on 2021/10/20.
//

#include "ICodeFactory.h"
#include "ICodeNodeImpl.h"

ICodeNode * ICodeFactory::createICodeNode(CTokenType::Token type) {
    return new ICodeNodeImpl(type);
}