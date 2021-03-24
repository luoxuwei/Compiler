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
