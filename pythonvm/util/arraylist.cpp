//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "arraylist.h"
#include <stdio.h>
#include "../runtime/Interpreter.h"

template<typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
    _data = new T[n];
    _size = 0;
}

template<typename T>
void ArrayList<T>::expend() {
    if (_size < _length) return;
    T* temp = new T[_length<<1];
    for (int i=0; i<_size; i++) {
        temp[i] = _data[i];
    }
    delete _data;
    _data = temp;
    _length = _length<<1;
    printf("expend an array to %d, size is %d\n", _length, _size);
}

template<typename T>
void ArrayList<T>::add(T t) {
    if (_size >= _length) {
        expend();
    }
    _data[_size++] = t;
}

template<typename T>
void ArrayList<T>::insert(int index, T t) {
    if (index > _size) return;
    add(NULL);
    for (int i = _size; i > index; i--) {
        _data[i] = _data[i-1];
    }
    _data[index] = t;
}

template<typename T>
int ArrayList<T>::size() {
    return _size;
}

template<typename T>
int ArrayList<T>::length() {
    return _length;
}

template<typename T>
T ArrayList<T>::get(unsigned int index) {
    if (index >= _size) return NULL;

    return _data[index];
}

template<typename T>
void ArrayList<T>::set(int index, T t) {
    if (_size <= index) {
        _size = index + 1;
    }

    while (_size > _length) {
        expend();
    }

    _data[index] = t;
}

template<typename T>
T ArrayList<T>::pop() {
    return _data[--_size];
}

template<typename T>
void ArrayList<T>::delete_index(int index) {
    for (int i=index; i+1<_size; i++) {
        _data[i] = _data[i+1];
    }
    _size--;
}

//由于arraylist声明在h文件，实现在cpp文件，所以编译器不会自动实例化模版类
//在这里进行声明强制编译器对模版类进行实例化
class PyObject;
template class ArrayList<PyObject*>;

class PyString;
template class ArrayList<PyString*>;

class Block;
template class ArrayList<Block*>;