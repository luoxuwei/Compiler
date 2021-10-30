//
// Created by 罗旭维 on 2021/10/28.
//

#include "PointerValueSetter.h"
#include "MemoryHeap.h"

PointerValueSetter::PointerValueSetter(Symbol *s, int i): symbol(s), index(i) {

}

void PointerValueSetter::setValue(Value *v) {
    Value *addr = symbol->getValue();
    Value::Buffer buf(0, 0, -1);
    MemoryHeap::getMem(addr->u.addr, buf);
    if (buf.size < 0) return;
    char *content = (char *) buf.buf;
    int sz = symbol->getByteSize();
    //如果是一个指向结构体的指针，每次写一个字节 struct TAG *pTag; pTag[0] = 1;
    if (symbol->getDeclarator(Declarator::POINTER) != NULL && symbol->getArgList() != NULL) {
        sz = 1;
    }
    if (sz == 4) {
        content[index] = (char) (v->u.i >> 24 & 0xff);
        content[index + 1] = (char) (v->u.i >> 16 & 0xff);
        content[index + 2] = (char) (v->u.i >> 8 & 0xff);
        content[index + 3] = (char) (v->u.i & 0xff);
    } else {
        content[index] = (char) (v->u.i & 0xff);
    }
}

Symbol * PointerValueSetter::getSymbol() {
    return symbol;
}