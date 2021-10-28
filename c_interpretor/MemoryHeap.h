//
// Created by 罗旭维 on 2021/10/28.
//

#ifndef C_COMPILER_MEMORYHEAP_H
#define C_COMPILER_MEMORYHEAP_H
#include <map>
#include "Value.h"

using namespace std;
//虚拟内存空间,模拟动态内存分配
class MemoryHeap {
private:
    static MemoryHeap *instance;
    static int initAddr;
    static map<int, Value::Buffer> memMap;
    MemoryHeap() {}
    MemoryHeap(const MemoryHeap &m) {}

public:
    static MemoryHeap *getInstance();
    static int allocMem(int size);
    static void getMem(int addr, Value::Buffer &b);
};


#endif //C_COMPILER_MEMORYHEAP_H
