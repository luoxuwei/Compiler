//
// Created by 罗旭维 on 2021/10/14.
//

#ifndef C_COMPILER_TYPELINK_H
#define C_COMPILER_TYPELINK_H
#include <stdio.h>

class TypeLink {
private:
    bool isTypeDef = false;//true，那么当前变量的类型是有typedef 定义的
    void * typeObject;
    TypeLink *next = NULL;
public:
    bool isDeclarator = true;//true 那么该object的对象是declarator, false那么object指向的是specifier
    TypeLink(bool isDeclarator, bool typeDef, void *typeObj);
    void * getTypeObject() {return typeObject;}
    TypeLink * toNext() {return next;}
    void setNextLink(TypeLink *obj) {next = obj;}
};


#endif //C_COMPILER_TYPELINK_H
