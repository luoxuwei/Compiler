//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#include "Klass.h"
#include "PyInteger.h"
#include "PyString.h"
#include "../runtime/universe.h"

int Klass::compare_klass(Klass *x, Klass *y) {
    if (x == y) {
        return 0;
    }

    if (x == IntegerKlass::get_instance()) {
        return -1;
    } else if (y == IntegerKlass::get_instance()) {
        return 1;
    }

    if (x->name()->less(y->name()) == Universe::PyTrue) {
        return -1;
    } else {
        return 1;
    }

}