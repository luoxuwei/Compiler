//
// Created by 罗旭维 on 2021/7/4.
//

#ifndef REGEX_UTIL_H
#define REGEX_UTIL_H
#include <assert.h>
#include <stdio.h>
#define errmsg(format, args...) fprintf(stderr, "in %s: "#format"\n", __FUNCTION__, ## args)
#define exceptionmsg(format, args...) { errmsg(format, ## args); assert(false); }

enum Error{
    E_MEM=0, //内存不足
    E_BADEXPR, //正则表达式错误
    E_PAREN,  //括号不匹配
    E_LENGTH, //要解析的正则表达式过多
    E_BRACKET, //字符集类没有以 [ 开头
    E_BOL, //^必须在表达式的开头
    E_CLOSE, //* ? + 后面必须跟着表达式
    E_NEWLINE, //双引号中不能保护换行符
    E_BADMAC, //没有匹配的 }
    E_NOMAC, //给定的宏表达式不存在
    E_MACDEPTH //宏表达式的间套太深
};

void parseErr(Error error);

#endif //REGEX_UTIL_H
