//
// Created by Xuwei Luo 罗旭维 on 2021/5/25.
//

#ifndef PYTHONVM_STACK_H
#define PYTHONVM_STACK_H

template<typename V>
class Stack {
private:
    V* vector;
    int _len;
    int _size;
public:
    Stack(int size = 16) {
        _size = 0;
        _len = size;
        vector = new V[size];
    }

    ~Stack() {
        delete[] vector;
        _len = 0;
        _size = 0;
    }

    V pop() {
        return vector[--_size];
    }

    V top() {
        return vector[_size - 1];
    }

    void push(V v) {
        vector[_size++] = v;
    }

    int len() {
        return _len;
    }

    int size() {
        return _size;
    }

    bool empty() {
        return _size == 0;
    }

};


#endif //PYTHONVM_STACK_H
