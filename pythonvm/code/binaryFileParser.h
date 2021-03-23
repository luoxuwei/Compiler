//
// Created by Xuwei Luo 罗旭维 on 2021/3/23.
//

#ifndef PYTHONVM_BINARYFILEPARSER_H
#define PYTHONVM_BINARYFILEPARSER_H
#include "../util/BufferInputStream.h"
#include "CodeObject.h"

class BinaryFileParser {
private:
    BufferInputStream* input;
public:
    binaryFileParser(BufferInputStream* file) {
        input = file;
    }

    CodeObject* parse();

};


#endif //PYTHONVM_BINARYFILEPARSER_H
