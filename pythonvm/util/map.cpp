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
V Map<K,V>::get(K k) {
    for (int i=0; i<_size; i++) {
        if (k == _value[i]->_k) {
            return _value[i]->_v;
        }
    }
    return NULL;
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
    return NULL;
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
    int index = index(k);
    if (index > 0) {
       V v = _value[index];
       _value[index] = _value[--_size];
    }
    return NULL;
}
