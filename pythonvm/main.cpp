#include <iostream>
#include "util/BufferInputStream.h"
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("vm need a paramter: filename\n");
        return 0;
    }
    char* path = argv[1];
    BufferInputStream bis(path);
    printf("magic number is 0x%x\n", bis.read_int());
    return 0;
}
