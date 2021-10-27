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

Value::Value(const Value &v) {
    type = v.type;
    u = v.u;
}

Value::Value(vector<Value *> *list) {
    type = LIST;
    u.list = list;
}

const char *Value::toString() {
    if (str != NULL) {
        delete str;
    }
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
            str = new string(*(u.s));
            return str->c_str();
    }
    return "";
}

bool Value::operator==(const Value &v) {
    if (type != v.type) return false;
    if (type == INT) {
        return u.i == v.u.i;
    } else if (type == LONG) {
        return u.l == v.u.l;
    } else if (type == FLOAT) {
        return u.f == v.u.f;
    } else if (type == STRING) {
        return *u.s == *v.u.s;
    }
}

bool Value::operator>=(const Value &v) {
    if (type != v.type) return false;
    if (type == INT) {
        return u.i >= v.u.i;
    } else if (type == LONG) {
        return u.l >= v.u.l;
    } else if (type == FLOAT) {
        return u.f >= v.u.f;
    } else if (type == STRING) {
        return *u.s >= *v.u.s;
    }
}

bool Value::operator>(const Value &v) {
    if (type != v.type) return false;
    if (type == INT) {
        return u.i > v.u.i;
    } else if (type == LONG) {
        return u.l > v.u.l;
    } else if (type == FLOAT) {
        return u.f > v.u.f;
    } else if (type == STRING) {
        return *u.s > *v.u.s;
    }
}

bool Value::operator<=(const Value &v) {
    if (type != v.type) return false;
    if (type == INT) {
        return u.i <= v.u.i;
    } else if (type == LONG) {
        return u.l <= v.u.l;
    } else if (type == FLOAT) {
        return u.f <= v.u.f;
    } else if (type == STRING) {
        return *u.s <= *v.u.s;
    }
}

bool Value::operator<(const Value &v) {
    if (type != v.type) return false;
    if (type == INT) {
        return u.i < v.u.i;
    } else if (type == LONG) {
        return u.l < v.u.l;
    } else if (type == FLOAT) {
        return u.f < v.u.f;
    } else if (type == STRING) {
        return *u.s < *v.u.s;
    }
}

void Value::operator++() {
    switch (type) {
        case INT:
            u.i++;
            break;
        case FLOAT:
            u.f++;
            break;
        case LONG:
            u.l++;
            break;
    }
    return;
}

void Value::operator--() {
    switch (type) {
        case INT:
            u.i--;
            break;
        case FLOAT:
            u.f--;
            break;
        case LONG:
            u.l--;
            break;
    }
    return;
}

Value::~Value() {
    if (str != NULL) {
        delete str;
    }

    if (type == STRING) {
        delete u.s;
    }

    if (type == LIST) {
        delete u.list;
    }
}