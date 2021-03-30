//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#include "map.h"
#include <stdio.h>
#include "../object/PyObject.h"
#include "../runtime/universe.h"

template<typename K, typename V>
MapEntry<K,V>::MapEntry(const MapEntry<K, V> &entry) {
    _k = entry._k;
    _v = entry._v;
}

template<typename K, typename V>
Map<K,V>::Map() {
    _value = new MapEntry<K,V>[8];
    _length = 8;
    _size = 0;
}

template<typename K, typename V>
V Map<K,V>::get(K k) {
    for (int i=0; i<_size; i++) {
        //坑太多了，开始为了快速开发直接k==_value[i]._k，
        // 后面忘了改用equal,导致运行一直有问题，单步调试好多次才发现 ！！！！
        if (_value[i]._k->equal(k) == (PyObject*)Universe::PyTrue) {
            return _value[i]._v;
        }
    }
    return Universe::PyNone;
}

template<typename K, typename V>
void Map<K, V>::expand() {
    if (_size >= _length) {
       MapEntry<K,V>* newValue = new MapEntry<K,V>[_length<<1];
       for (int i=0; i<_size; i++) {
           newValue[i] = _value[i];
       }
       delete[] _value;
       _value = newValue;
       _length = _length<<1;
    }
}

template<typename K, typename V>
void Map<K,V>::put(K k, V v) {
    for (int i=0; i<_size; i++) {
        if (_value[i]._k->equal(k) == (PyObject*)Universe::PyTrue) {
            _value[i]._v = v;
            return;
        }
    }
    expand();
    _value[_size++] = MapEntry<K,V>(k,v);
}

template<typename K, typename V>
K Map<K,V>::get_key(int index) {
    if (index < _size) {
        return _value[index]._k;
    }
    return Universe::PyNone;
}

template<typename K, typename V>
K Map<K,V>::get_value(int index) {
    if (index < _size) {
        return _value[index]._v;
    }
    return Universe::PyNone;
}

template<typename K, typename V>
int Map<K,V>::index(K k) {
    for (int i=0; i<_size; i++) {
        if (_value[i]._k->equal(k) == (PyObject*)Universe::PyTrue) {
            return i;
        }
    }
    return -1;
}

template<typename K, typename V>
bool Map<K,V>::has_key(K k) {
    return index(k) != -1;
}

template<typename K, typename V>
V Map<K,V>::remove(K k) {
    int i = index(k);
    if (i > 0) {
       V v = _value[i]._v;
       _value[i] = _value[--_size];
        return v;
    }
    return Universe::PyNone;
}

//强制编译器实例化，不然编译链接时会报Undefined symbols
template class Map<PyObject*, PyObject*>;