//
// Created by 罗旭维 on 2021/10/28.
//

#include "DirectMemValueSetter.h"
#include "MemoryHeap.h"
#include "Value.h"

void DirectMemValueSetter::setValue(Value *v) {
    Value::Buffer buffer(0, 0, -1);
    MemoryHeap::getMem(memAddr, buffer);
    char *content = (char *) buffer.buf;
    int offset = memAddr - buffer.addr;
    content[offset] = v->u.i & 0xff;
}

Symbol * DirectMemValueSetter::getSymbol() {
    return NULL;
}