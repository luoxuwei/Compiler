//
// Created by 罗旭维 on 2021/10/28.
//

#include "MemoryHeap.h"

int MemoryHeap::initAddr = 10000;
MemoryHeap *MemoryHeap::instance = NULL;
map<int, Value::Buffer> MemoryHeap::memMap = map<int, Value::Buffer>();

MemoryHeap * MemoryHeap::getInstance() {
    if (instance == NULL) {
        instance = new MemoryHeap();
    }
    return instance;
}

int MemoryHeap::allocMem(int size) {
    void *mem = malloc(size);
    memMap[initAddr] = Value::Buffer(mem, initAddr, size);
    int allocAddr = initAddr;
    initAddr += size;
    return allocAddr;
}

void MemoryHeap::getMem(int addr, Value::Buffer &b) {
    int initAddr = 0;
    for (auto iter : memMap) {
        //比传入的addr小，并且是最大的一个
        if (iter.first <= addr && iter.first > initAddr) {
            initAddr = iter.first;
            Value::Buffer mems = iter.second;
            if (initAddr + mems.size > addr) {
                b = mems;
            }
        }
    }
}