//
// Created by 罗旭维 on 2021/10/22.
//

#ifndef C_COMPILER_IVALUESETTER_H
#define C_COMPILER_IVALUESETTER_H
#include "Value.h"
//对一个对象赋值的时候，这个对象可能是单一的变量如int a这种，有可能赋值的是数组中某一个个元素，还有可能是结构体中的某一个域
//这样当我们给某个对象赋值时，不知道具体是哪一个，因此将对不同对象的赋值的操作，通过单一的接口抽象起来
//具体对哪个对象赋值的操作可能不一样，比如对单一变量的赋值动作和对数组元素的赋值动作就不一样，但这个动作是怎么执行的，在高层不管，只要调用这个接口的setValue就行了
//不同的赋值对象实现这个接口即可
class Symbol;
class IValueSetter {
public:
    virtual void setValue(Value *v) = 0;
    virtual Symbol *getSymbol() = 0;
};


#endif //C_COMPILER_IVALUESETTER_H
