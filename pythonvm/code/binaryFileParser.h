//
// Created by Xuwei Luo 罗旭维 on 2021/3/23.
//

#ifndef PYTHONVM_BINARYFILEPARSER_H
#define PYTHONVM_BINARYFILEPARSER_H
#include "../util/BufferInputStream.h"
#include "CodeObject.h"
#include "../object/PyString.h"
#include "../util/arraylist.h"
#include "../object/PyInteger.h"
#include "../object/PyObject.h"
#include "../util/arraylist.h"

class BinaryFileParser {
private:
    BufferInputStream* input;
    ArrayList<PyString*> _string_table;
public:
    BinaryFileParser(BufferInputStream* file) {
        input = file;
    }

    CodeObject* parse();
    CodeObject* get_code_object();
    PyString* get_byte_codes();
    PyString* get_string();
    ArrayList<PyObject*>* get_consts();
    ArrayList<PyObject*>* get_names();
    ArrayList<PyObject*>* get_var_names();
    ArrayList<PyObject*>* get_free_vars();
    ArrayList<PyObject*>* get_cell_vars();
    ArrayList<PyObject*>* get_tuple();
    PyString* get_file_name();
    PyString* get_name();
    PyString* get_no_table();
};


#endif //PYTHONVM_BINARYFILEPARSER_H
