//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_ARRAYLIST_H
#define PYTHONVM_ARRAYLIST_H
#include <stdio.h>
template <typename T>
class ArrayList {
private:
    T* _data;
    int _length;
    int _size;
    void expend();

public:
    ArrayList(int n);
    void add(T t);
    void insert(int index, T t);
    T get(unsigned index);
    void set(int index, T t);
    int size();
    int length();
    T pop();
};


#endif //PYTHONVM_ARRAYLIST_H
