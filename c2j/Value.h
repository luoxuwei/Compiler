//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_VALUE_H
#define C_COMPILER_VALUE_H
#include <string>
#include <vector>
using namespace std;
class Value {
public:
    struct Buffer {
        int addr = -1;
        int size = 0;
        void *buf = NULL;
        Buffer() {}
        Buffer(void *b, int addr, int size) {
            buf = b;
            this->addr = addr;
            this->size = size;
        }
    };

    union {
        int i;
        long l;
        float f;
        string *s;
        vector<Value *> *list;
        int addr;
    } u;

    enum ValueType {
        FLOAT,
        INT,
        LONG,
        STRING,
        LIST,
        POINTER
    };

    ValueType type;
    string *str = NULL;
    Value();
    Value(int v);
    Value(string &s);
    Value(long l);
    Value(float f);
    Value(vector<Value *> *list);
    Value(ValueType type, int i);
    Value(const Value &v);
    const char *toString();
    ~Value();
    bool operator==(const Value &v);
    bool operator>=(const Value &v);
    bool operator>(const Value &v);
    bool operator<=(const Value &v);
    bool operator<(const Value &v);
    void operator++();
    void operator--();
};


#endif //C_COMPILER_VALUE_H
