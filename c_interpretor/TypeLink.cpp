//
// Created by 罗旭维 on 2021/10/14.
//

#include "TypeLink.h"

TypeLink::TypeLink(bool isDeclarator, bool typeDef, void *typeObj) : isDeclarator(isDeclarator), isTypeDef(typeDef), typeObject(typeObj) {}