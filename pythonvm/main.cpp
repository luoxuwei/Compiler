#include <iostream>
#include "util/BufferInputStream.h"
#include "code/binaryFileParser.h"

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
    BufferInputStream bis(path);
    BinaryFileParser fileParser(&bis);
    fileParser.parse();
    return 0;
}
