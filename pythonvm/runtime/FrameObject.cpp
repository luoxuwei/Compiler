//
// Created by Xuwei Luo 罗旭维 on 2021/3/26.
//

#include "FrameObject.h"
#include "../object/PyList.h"
#include "../object/PyDict.h"

FrameObject::FrameObject() {

}

//这个构造函数是用于创建第一帧，这里没有新建一个变量表，而是直接
//用_locals，_locals和_globals指向同一个对象，这么做的原因是在非函数上下文中
//python的局部变量与全局变量的作用域是一样的，只有在调用函数时，创建了新的栈帧，才对局部变量和全局变量进行区分
FrameObject::FrameObject(CodeObject *code) {
    _consts = code->_consts;
    _names = code->_names;
    _locals = new Map<PyObject*, PyObject*>();
    _stack = new ArrayList<PyObject*>();
    _loop_stack = new ArrayList<Block*>();
    _globals = _locals; //
    _codes = code;
    _pc = 0;
    _sender = NULL;
}

FrameObject::FrameObject(FunctionObject *functionObject, ArrayList<PyObject*>* args, int op_arg) {

    _locals = new Map<PyObject*, PyObject*>();
    _stack = new ArrayList<PyObject*>();
    _loop_stack = new ArrayList<Block*>();
    _codes = functionObject->_func_code;
    _consts = _codes->_consts;
    _names = _codes->_names;
    _globals = functionObject->_globals;
    _pc = 0;
    _sender = NULL;
    _fast_locals = new ArrayList<PyObject*>();
    const int argcnt = _codes->_argcount;
    const int na = op_arg & 0xff;//
    const int nk = op_arg >> 8;
    int kw_pos = argcnt;//argcnt是形式参数或位置参数的个数

    if (functionObject->_defaults) {
        int dft_cnt = functionObject->_defaults->size();
        int argnum = _codes->_argcount;
        while (dft_cnt--) {
            //这里_fast_locals 的插入位置用--argcnt，而不是dft_cnt--，因为python语法规定
            //默认参数必须定义在非默认参数之前 def foo(a=1, b)，这样的定义是不合法的，
            //这样的语法保证了在处理默认值的时候，从后往前填入默认值的做法是绝对正确的。
            _fast_locals->set(--argnum, functionObject->_defaults->get(dft_cnt));
        }
    }

    if (args != NULL) {
        PyList* alist = NULL;//存扩展位置参数
        PyDict* adict = NULL;//存扩展键参数

        //argcnt代表了函数定义时规定的形式参数，或者叫位置参数的个数，实际上就是参数个数，因为扩展位置和扩展键参数都不算参数个数。
        //na代表了实际传入的参数个数。如果实际传入的参数比函数的形式参数多，就把不超出的那部分放到fast_locals表里，把多出来的那部分放到
        //alist， 也就是扩展位置参数中。
        if (argcnt < na) {
            int i = 0;
            for (; i<argcnt; i++) {
                _fast_locals->set(i, args->get(i));
            }
            alist = new PyList();
            for (; i<na; i++) {
                alist->append(args->get(i));
            }
        } else {
            for (int i=0; i<args->size(); i++) {
                _fast_locals->set(i, args->get(i));
            }
        }

        //处理键参数的逻辑，键名称为key，代表参数名称，val代表实际的参数值。
        for (int i=0; i < nk; i++) {
            PyObject* key = args->get(na + i*2);
            PyObject* val = args->get(na + i*2+1);

            //查找该参数名在函数定义时的序号，然后把值放到fast_locals表的相应序号处。
            //如果没找到就把这一组键值对放到扩展键值参数中去。
            int index = _codes->_var_names->index(key);
            if (index > 0) {
                _fast_locals->set(index, val);
            } else {
                if (adict == NULL) adict = new PyDict();
                adict->put(key, val);
            }
        }

        //处理扩展位置参数，如果flags的CO_VARARGS置位，证明该函数有扩展位置参数
        if (functionObject->_flags & FunctionObject::CO_FLAGS::CO_VARARGS) {
            if (alist == NULL)
                alist = new PyList();

            _fast_locals->set(argcnt, alist);
            kw_pos += 1;
        } else {
           if (alist != NULL) {
               printf("takes more extend parameters.\n");
               assert(false);
           }
        }

        //处理扩展键参数
        if (functionObject->_flags & FunctionObject::CO_FLAGS::CO_VARKEYWORDS) {
            if (adict == NULL) {
                adict = new PyDict();
            }

            _fast_locals->set(kw_pos, adict);
        } else {
            if (adict != NULL) {
                printf("takes more extend kw parameters.\n");
                assert(false);
            }
        }
    }
}

int FrameObject::get_op_arg() {
    int byte1 = (_codes->_bytecodes->value()[_pc++] & 0xFF);
    return ((_codes->_bytecodes->value()[_pc++] & 0xFF) << 8) | byte1;
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}