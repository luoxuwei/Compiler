//
// Created by Xuwei Luo 罗旭维 on 2021/3/23.
//

#include "binaryFileParser.h"

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
    return NULL;
}
