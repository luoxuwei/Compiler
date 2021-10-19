//
// Created by 罗旭维 on 2021/10/15.
//

#ifndef C_COMPILER_DECLARATOR_H
#define C_COMPILER_DECLARATOR_H


class Declarator {
public:
    const static int POINTER = 0;
    const static int ARRAY = 1;
    const static int FUNCTION = 2;

    Declarator(int type);

    void setElementNum(int num);

    int getType();

    int getElementNum();

private:
    int declareType = POINTER;
    int numberOfElements = 0;
};


#endif //C_COMPILER_DECLARATOR_H
