//
// Created by Xuwei Luo 罗旭维 on 2021/5/24.
//

#ifndef PYTHONVM_HEAP_H
#define PYTHONVM_HEAP_H
#include <memory>

//GC算法一个重要的功能是识别出内存中的哪些对象是垃圾。所谓垃圾就是不再被引用的对象。
//引用计数，每个对象引入计数器，来记录有多少个其他对象引用了它，python中赋值语句最终会被翻译成STORE_XX指令，
//可以在执行STORE指令时，改变对象的引用计数，在一次赋值中把新对象引用计数加一，老对象引用计数减一，如果某个
//对象引用计数为0就把这个对象回收掉，然后把这个对象所引用的所有对象的引用计数减1。
//不能先减后加，因为如果赋值两边是同一个对象时，如果恰好这个对象的引用计数为1，先减后加这个对象会被回收，发生错误。
//引用计数法不能解决循环引用的问题，A引用B，B引用A，除此之外没有其他地方引用这两个对象，这两个对象的引用计数就都为1，这种情况下
//这两个对象是不能被回收的。为了解决循环引用的问题，一类基于引用追踪的算法便应运而生。Tracing GC
//这类算法的特点是以roots集合为起点进行图遍历，如果从roots到某个对象是可达的，则该对象称为"可达对象"，也就是还活着的对象
//否则就是不可达对象，可以被回收。所有不在堆里但又指向堆里某一个对象的引用，这些引用组成一个集合，称之为roots。
//Tracing GC 中一个典型的算法是Mark-Sweep算法，这个算法有内存碎片的问题，
//Copy GC 算法把堆分成大小相同的两半，一半称为from空间，一半称为to空间。利用from空间进行分配，空间不足触发GC，会把
//from空间中存活的对象全部复制到to空间，复制完了把from和to空间互换。
//Space代表一个独立的空间，复制回收算法中的空间。
class Space {
    friend class Heap;
private:
    char* _base;//起始地址
    char* _top;//当前可用内存开始地址
    char* _end;//尾地址
    size_t _size;//总容量 = _end - _base
    size_t _capacity;//当前可用内存容量 = _end-_top
    Space(size_t size);
    ~Space();

public:
    void* allocate(size_t size);
    void clear();
    bool can_alloc(size_t size);
    bool has_obj(char* obj);
};

//Heap类代表虚拟机堆，包含三个空间，survivor，eden，meta，
//把klass放到meta空间，meta空间里的信息相对稳定，不需要频繁回收，放klass比较合适。
class Heap {
private:
    Space* mem_1;
    Space* mem_2;

    Space* eden;
    Space* survivor;
    Space* metaspace;

    Heap(size_t size);

public:
    static size_t MAX_CAP;
    static Heap* instance;
    static Heap* get_instance();

    ~Heap();
    void* allocate(size_t size);
    void* allocate_meta(size_t size);
    void copy_live_objects();
    void gc();
};


#endif //PYTHONVM_HEAP_H
