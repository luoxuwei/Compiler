//
// Created by Xuwei Luo 罗旭维 on 2021/4/6.
//

#ifndef PYTHONVM_STRINGTABLE_H
#define PYTHONVM_STRINGTABLE_H
#include "../object/PyString.h"

class StringTable {
private:
    static StringTable* instance;
    StringTable();

public:
    static StringTable* get_instance();
    PyString* next_str = NULL;
    PyString* mod_str = NULL;
    PyString* init_str = NULL;

};


#endif //PYTHONVM_STRINGTABLE_H
