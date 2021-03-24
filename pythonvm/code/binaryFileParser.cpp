//
// Created by Xuwei Luo 罗旭维 on 2021/3/23.
//

#include "binaryFileParser.h"
#include <assert.h>

CodeObject * BinaryFileParser::parse() {
    int magic_number = input->read_int();
    printf("magic number is 0x%x\n", magic_number);
    int moddate = input->read_int();
    printf("moddate is 0x%x\n", moddate);

    char object_type = input->read();
    if (object_type == 'c') {
        CodeObject* result = get_code_object();
        printf("parse OK!\n");
        return result;
    }

    return NULL;
}

CodeObject * BinaryFileParser::get_code_object() {
    int argcount = input->read_int();
    int nlocals = input->read_int();
    int stackSize = input->read_int();
    int flags = input->read_int();

    PyString* byte_codes = get_byte_codes();
    ArrayList<PyObject*>* consts = get_consts();
    return NULL;
}

PyString * BinaryFileParser::get_byte_codes() {
    assert(input->read() == 's');
    return get_string();
}

PyString * BinaryFileParser::get_string() {
    int len = input->read_int();
    char* str_value = new char[len];
    for (int i=0; i<len; i++) {
        str_value[i] = input->read();
    }
    PyString* result = new PyString(str_value, len);
    delete[] str_value;
    return result;
}

ArrayList<PyObject *> * BinaryFileParser::get_consts() {
    if (input->read() == '(') {
        return get_tuple();
    }

    input->unread();
    return NULL;
}

ArrayList<PyObject*>* BinaryFileParser::get_tuple() {
    int num = input->read_int();
    ArrayList<PyObject*>* list = new ArrayList<PyObject*>(num);
    PyString* str;
    for (int i=0; i<num; i++) {
        char obj_type = input->read();
        switch (obj_type) {
            case 'i':
                list->add(new PyInteger(input->read_int()));
                break;
            case 's':
                list->add(get_string());
                break;
            case 'c':
                list->add(get_code_object());
                break;
            case 'N':
                list->add(NULL);
                break;
            case 't':
                str = get_string();
                list->add(str);
                _string_table.add(str);
                break;
            case 'R':
                list->add(_string_table.get(input->read_int()));
                break;

        }
    }
    return list;
}


