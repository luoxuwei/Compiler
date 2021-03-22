//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_PYINTEGER_H
#define PYTHONVM_PYINTEGER_H


class PyInteger {
private:
    int value;
public:
    PyInteger(int v) {value = v;}
    int value() {return value;}
};


#endif //PYTHONVM_PYINTEGER_H
