//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_CODEOBJECT_H
#define PYTHONVM_CODEOBJECT_H
#include "../object/PyObject.h"

/*
 * pyc文件格式：
 * 开头四个字节是魔数，用来标识文件类型和版本，低字节存低位(小端)，接着四个字节是创建时间，接着一个字节是字符'c' 0x63，
 * 表示接下来是一个codeobject结构，codeobject第一个域是一个整数，是参数个数，第二个域也是一个整数代表局部变量个数，第三个域
 * 是一个整数，代表执行这段code所使用的操作数栈，第四个域code的属性也是一个整数，接着就是字节码，以s字符开始，接着一个整数，代表
 * 这段字节码的长度，接下来的这段数据就都是字节码。
 * */
class PyString;

template<typename T>
class ArrayList;

class CodeObject {
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
