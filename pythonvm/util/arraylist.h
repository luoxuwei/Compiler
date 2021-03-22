//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_ARRAYLIST_H
#define PYTHONVM_ARRAYLIST_H
#include <stdio.h>
template <typename T>
class Arraylist {
private:
    T* _data;
    int _length;
    int _size;
    expend();

public:
    Arraylist(int n);
    add(T t);
    insert(int index, T t);
    T get(int index);
    void set(int index, T t);
    int size();
    int length();
    T pop();
};


#endif //PYTHONVM_ARRAYLIST_H
