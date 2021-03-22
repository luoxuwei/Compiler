//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "arraylist.h"
#include <stdio.h>

template<typename T>
Arraylist<T>::Arraylist(int n) {
    _length = n;
    _data = new T[n];
    _size = 0;
}

template<typename T>
void Arraylist<T>::expend() {
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
void Arraylist<T>::add(T t) {
    if (_size >= _length) {
        expend();
    }
    _data[size++] = t;
}

template<typename T>
void Arraylist<T>::insert(int index, T t) {
    if (index > _size) return;
    add(NULL);
    for (int i = _size; i > index; i--) {
        _data[i] = _data[i-1];
    }
    _data[index] = t;
}

template<typename T>
int Arraylist<T>::size() {
    return _size;
}

template<typename T>
int Arraylist<T>::length() {
    return _length;
}

template<typename T>
T Arraylist<T>::get(unsigned int index) {
    if (index >= _size) return NULL;

    return _data[index];
}

template<typename T>
void Arraylist<T>::set(int index, T t) {
    if (_size <= index) {
        _size = index + 1;
    }

    while (_size > _length) {
        expend();
    }

    _data[index] = t;
}


