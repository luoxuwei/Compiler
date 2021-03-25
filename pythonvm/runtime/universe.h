//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#ifndef PYTHONVM_UNIVERSE_H
#define PYTHONVM_UNIVERSE_H
#include "../object/PyInteger.h"

class Universe {
public:
    static PyInteger* PyTrue;
    static PyInteger* PyFalse;
    static PyObject* PyNone;

public:
    static void genesis();//这里创建虚拟机最原始的对象结构，虚拟机里所有的对象以后都会以这个方法为起点
    static void destroy();//这个方法在虚拟机退出是调用，销毁对象，释放资源和清理空间。
};


#endif //PYTHONVM_UNIVERSE_H