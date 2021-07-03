#include <iostream>
#include "Regex.h"

int main(int argc, char** argv) {
    if (argc > 2) {
        Regex regex(argv[1]);
        regex.printMacs();
        regex.parse(argv[2]);
        regex.printExpr();
    }
    return 0;
}
