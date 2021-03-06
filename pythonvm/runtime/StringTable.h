//
// Created by Xuwei Luo 罗旭维 on 2021/4/6.
//

#ifndef PYTHONVM_STRINGTABLE_H
#define PYTHONVM_STRINGTABLE_H
#include "../object/PyString.h"
#include "../memory/oopClosure.h"

class StringTable {
private:
    static StringTable* instance;
    StringTable();

public:
    static StringTable* get_instance();
    PyString* next_str = NULL;
    PyString* mod_str = NULL;
    PyString* init_str = NULL;
    PyString* add_str = NULL;
    PyString* len_str = NULL;
    PyString* call_str = NULL;
    PyString* getitem_str = NULL;
    PyString* setitem_str = NULL;
    PyString* setattr_str = NULL;
    PyString* getattr_str = NULL;
    void oops_do(OopClosure* closure);
};


#endif //PYTHONVM_STRINGTABLE_H
