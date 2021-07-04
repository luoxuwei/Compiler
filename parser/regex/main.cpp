#include <iostream>
#include "ExprLexer.h"

int main(int argc, char** argv) {
    if (argc > 2) {
        ExprLexer lexer(argv[1], argv[2]);
        lexer.printMacs();
        lexer.printExpr();
    }
    return 0;
}
