//
// Created by 罗旭维 on 2021/10/21.
//

#include "Value.h"
Value::Value() {
    u.i = 0;
    type = INT;
}

Value::Value(int v) {
    type = INT;
    u.i = v;
}

Value::Value(string &s) {
    type = STRING;
    u.s = new string(s);
}

Value::Value(long l) {
    type = LONG;
    u.l = l;
}

Value::Value(float f) {
    type = FLOAT;
    u.f = f;
}

const char *Value::toString() {
    if (str != NULL) return str->c_str();
    switch (type) {
        case ValueType::FLOAT:
            str = new string(to_string(u.f));
            return str->c_str();
        case ValueType::INT:
            str = new string(to_string(u.i));
            return str->c_str();
        case ValueType::LONG:
            str = new string(to_string(u.l));
            return str->c_str();
        case ValueType::STRING:
            str = u.s;
            return str->c_str();
    }
    return "";
}

Value::~Value() {
    if (str != NULL) {
        delete str;
    }

    if (type == STRING) {
        delete u.s;
    }
}