//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_CODEOBJECT_H
#define PYTHONVM_CODEOBJECT_H
#include "../object/PyObject.h"
#include "../util/arraylist.h"
#include "../object/PyString.h"

/*
 * pyc文件格式：
 * 开头四个字节是魔数，用来标识文件类型和版本，低字节存低位(小端)，接着四个字节是创建时间，接着一个字节是字符'c' 0x63，
 * 表示接下来是一个codeobject结构，codeobject第一个域是一个整数，是参数个数，第二个域也是一个整数代表局部变量个数，第三个域
 * 是一个整数，代表执行这段code所使用的操作数栈，第四个域code的属性也是一个整数，接着就是字节码，以s字符开始，接着一个整数，代表
 * 这段字节码的长度，接下来的这段数据就都是字节码。
 *
 * 字节码之后是常量表，是一个元组以'('开头，接着一个整数表示元素个数, 每个元素开始是一个表示数据类型的字符，整数是i，字符串是s，N是none等，
 * 常量表之后是，变量表，也是以'('开头，接着一个整数表示元素个数，变量之后是参数列表，一样也是元组形式，再后面分别是cell var 和 free var
 * 用户构造闭包，接下来是文件名，是一个字符串以's'开头，接着一个整数表示字符串长度，后面就是这个字符串的字符数据，接下来是一个字符't'也是一个字符串
 * 与's'的格式相同，代表当前code所属的模块，最后一项也是一个字符串以's'标志，是一个名为line number table的结构，在实现traceback时，要输出详细的
 * 调用栈，会用到这个数据结构。
 * */


class CodeKlass: public Klass {
private:
    static CodeKlass* instance;
    CodeKlass();

public:
    static CodeKlass* get_instance();
};

class CodeObject: public PyObject {
public:
    int _argcount;//参数个数
    int _nlocals;//本地变量个数
    int _stacks_size;//操作数栈深度
    int _flag;//code属性

    PyString* _bytecodes;
    ArrayList<PyObject*>* _names;
    ArrayList<PyObject*>* _consts;
    ArrayList<PyObject*>* _var_names;

    ArrayList<PyObject*>* _free_vars;
    ArrayList<PyObject*>* _cell_vars;

    PyString* _co_name;
    PyString* _file_name;

    int _lineno;
    PyString* _notable;

    CodeObject(int argcount, int nlocals, int stacksize, int flag, PyString* bytecodes,
               ArrayList<PyObject*>* consts, ArrayList<PyObject*>* names,
               ArrayList<PyObject*>* varnames, ArrayList<PyObject*>* freevars, ArrayList<PyObject*>* cellvars,
               PyString* file_name, PyString* co_name, int lineno, PyString* notable);
};


#endif //PYTHONVM_CODEOBJECT_H
