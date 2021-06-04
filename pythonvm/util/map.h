//
// Created by Xuwei Luo 罗旭维 on 2021/3/25.
//

#ifndef PYTHONVM_MAP_H
#define PYTHONVM_MAP_H
class OopClosure;
template<typename K, typename V>
class MapEntry {
public:
    V _v;
    K _k;

    MapEntry():_v(0),_k(0){}
    MapEntry(K k, V v):_k(k), _v(v) {}
    MapEntry(const MapEntry<K, V>& entry);

};

template<typename K, typename V>
class Map {
private:
    MapEntry<K, V>* _value;
    int _size;
    int _length;
    void expand();

public:
    Map();

    int size() {return _size;}
    void put(K k, V v);
    V get(K k);
    K get_key(int index);
    K get_value(int index);
    bool has_key(K k);
    V remove(K k);
    int index(K k);
    MapEntry<K, V>* entries() {return _value;};
    void oops_do(OopClosure* oopClosure);
};


#endif //PYTHONVM_MAP_H
