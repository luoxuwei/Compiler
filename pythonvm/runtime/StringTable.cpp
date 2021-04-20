//
// Created by Xuwei Luo 罗旭维 on 2021/4/6.
//

#include "StringTable.h"

StringTable* StringTable::instance = NULL;

StringTable * StringTable::get_instance() {
    if (instance == NULL) {
        instance = new StringTable();
    }
    return instance;
}

StringTable::StringTable() {
    next_str = new PyString("next");
}