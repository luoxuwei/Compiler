//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_VALUE_H
#define C_COMPILER_VALUE_H
#include <string>

using namespace std;
class Value {
public:
    union {
        int i;
        long l;
        float f;
        string *s;
    } u;

    enum ValueType {
        FLOAT,
        INT,
        LONG,
        STRING
    };

    ValueType type;
    string *str = NULL;
    Value();
    Value(int v);
    Value(string &s);
    Value(long l);
    Value(float f);
    const char *toString();
    ~Value();
};


#endif //C_COMPILER_VALUE_H
