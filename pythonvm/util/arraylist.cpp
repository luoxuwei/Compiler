//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#include "arraylist.h"
#include <stdio.h>
#include "../runtime/Interpreter.h"
#include "../runtime/universe.h"
#include "../memory/heap.h"

template<typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
//    _data = new T[n];
//我们不知道T的具体类型所以不能为T增加数组new 操作。这里就需要一种新的技巧，那就是placement new,也称为定位new。
//在已分配的原始内存中初始化一个对象，它与new的其他版本的不同之处在于，它不分配内存。相反它接受一个已经分配好的内存地址，
//然后在这块内存里初始化一个对象，这就使其能够在特定的预分配的内存地址中构造一个对象。简单来说就是定位new可以让我们有办法单独地调用构造函数。
    void* temp = Universe::heap->allocate(sizeof(T)*n);
    _data = new(temp)T[n];
    _size = 0;
}

template<typename T>
void ArrayList<T>::expend() {
    if (_size < _length) return;
//    T* temp = new T[_length<<1];
    void* temp = Universe::heap->allocate(sizeof(T)*(_length<<1));
    T* new_array = new(temp)T[_length<<1];
    for (int i=0; i<_size; i++) {
        new_array[i] = _data[i];
    }
//    delete _data;
    _data = new_array;
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

template<typename T>
T ArrayList<T>::top() {
    return _data[_size-1];
}

template<typename T>
int ArrayList<T>::index(T t) {
    return 0;
}

template<>
int ArrayList<PyObject*>::index(PyObject *t) {
    for (int i = 0; i < _size; i++) {
        if (_data[i]->equal(t) == Universe::PyTrue) {
            return i;
        }
    }
    return -1;
}

template<typename T>
void ArrayList<T>::oops_do(OopClosure *closure) {
    closure->do_raw_mem((char **)&_data, _length*sizeof(T));
}

//使用模版偏特化
template<>
void ArrayList<Klass*>::oops_do(OopClosure *closure) {
    closure->do_raw_mem((char **)&_data, _length*sizeof(Klass*));
    for (int i=0; i<_size; i++) {
        closure->do_klass((Klass**)&_data[i]);
    }
}

template<>
void ArrayList<PyObject*>::oops_do(OopClosure *closure) {
    closure->do_raw_mem((char **)&_data, _length*sizeof(PyObject*));
    for (int i=0; i<_size; i++) {
        closure->do_oop((PyObject**)&_data[i]);
    }
}

template<typename T>
void * ArrayList<T>::operator new(size_t size) {
    return Universe::heap->allocate(size);
}


//由于arraylist声明在h文件，实现在cpp文件，所以编译器不会自动实例化模版类
//在这里进行声明强制编译器对模版类进行实例化
class PyObject;
template class ArrayList<PyObject*>;

class PyString;
template class ArrayList<PyString*>;

class Block;
template class ArrayList<Block*>;

class Klass;
template class ArrayList<Klass*>;