//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#ifndef PYTHONVM_UNIVERSE_H
#define PYTHONVM_UNIVERSE_H
#include "../object/PyObject.h"
#include "../code/CodeObject.h"
#include "../util/arraylist.h"

class Klass;
class Heap;

class Universe {
public:
    static PyObject* PyTrue;
    static PyObject* PyFalse;
    static PyObject* PyNone;
    static CodeObject* main_code;
    static ArrayList<Klass*>* klasses;
    static Heap* heap;

public:
    static void genesis();//这里创建虚拟机最原始的对象结构，虚拟机里所有的对象以后都会以这个方法为起点
    static void destroy();//这个方法在虚拟机退出是调用，销毁对象，释放资源和清理空间。
    static void oops_do(OopClosure* closure);
};


#endif //PYTHONVM_UNIVERSE_H
