#include <iostream>
#include "Regex.h"

int main(int argc, char** argv) {
    if (argc > 1) {
        Regex regex(argv[1]);
        regex.printMacs();
    }
    return 0;
}
