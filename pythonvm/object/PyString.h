//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_PYSTRING_H
#define PYTHONVM_PYSTRING_H


class PyString {
private:
    char* _value;
    unsigned int _len;
public:
    //可以使用strlen获取字符串长度
    PyString(const char* x);
    //'\0'也是允许的字符时不能使用strlen，只能通过参数传入长度
    PyString(const char* x, const int len);

    const char* value() {return _value;}
    int length() {return _len;}
};


#endif //PYTHONVM_PYSTRING_H
