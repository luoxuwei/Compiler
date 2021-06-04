#include <iostream>
#include "util/BufferInputStream.h"
#include "code/binaryFileParser.h"
#include "runtime/Interpreter.h"
#include "runtime/universe.h"
#include "object/PyDict.h"

//g++ -o test -g main.cpp
//./test hello.pyc
//argv 0 是可执行文件本身的路径

//g++ -o test -g *.cpp
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("vm need a paramter: filename\n");
        return 0;
    }
    char* path = argv[1];
    Universe::genesis();
    BufferInputStream bis(path);
    BinaryFileParser fileParser(&bis);
    Universe::main_code = fileParser.parse();
    Interpreter::get_instance()->run(Universe::main_code);
    return 0;
}
