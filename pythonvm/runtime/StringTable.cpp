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
    mod_str = new PyString("__module__");
    init_str = new PyString("__init__");
    add_str = new PyString("__add__");
    len_str = new PyString("__len__");
    call_str = new PyString("__call__");
    getitem_str = new PyString("__getitem__");
    setitem_str = new PyString("__setitem__");
}