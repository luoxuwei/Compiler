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
    union {
        int i;
        long l;
        float f;
        string *s;
        vector<Value *> *list;
    } u;

    enum ValueType {
        FLOAT,
        INT,
        LONG,
        STRING,
        LIST
    };

    ValueType type;
    string *str = NULL;
    Value();
    Value(int v);
    Value(string &s);
    Value(long l);
    Value(float f);
    Value(vector<Value *> *list);
    Value(const Value &v);
    const char *toString();
    ~Value();
    bool operator==(const Value &v);
    bool operator>=(const Value &v);
    bool operator>(const Value &v);
    bool operator<=(const Value &v);
    bool operator<(const Value &v);
    void operator++();
};


#endif //C_COMPILER_VALUE_H
