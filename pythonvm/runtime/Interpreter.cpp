//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#include "Interpreter.h"
#include "../code/bytecode.h"
#include "../object/PyInteger.h"
#include "universe.h"
#include "FunctionObject.h"
#include "../object/PyList.h"
#include "StringTable.h"
#include "../object/PyDict.h"
#include "cellObject.h"
#include "../object/pytypeobject.h"

#define PUSH(x) _frame->_stack->append(x)
#define POP() _frame->_stack->pop()
#define TOP() _frame->_stack->top()
#define STACK_LEVEL() _frame->_stack->size()
#define PY_TRUE Universe::PyTrue
#define PY_FALSE Universe::PyFalse

Interpreter::Interpreter() {
    _builtins = new Map<PyObject*, PyObject*>();
    _builtins->put(new PyString("True"), Universe::PyTrue);
    _builtins->put(new PyString("False"), Universe::PyFalse);
    _builtins->put(new PyString("None"), Universe::PyNone);
    _builtins->put(new PyString("len"), new FunctionObject(len));
    _builtins->put(new PyString("int"), IntegerKlass::get_instance()->type_object());
    _builtins->put(new PyString("object"), ObjectKlass::get_instance()->type_object());
    _builtins->put(new PyString("list"), ListKlass::get_instance()->type_object());
    _builtins->put(new PyString("str"), StringKlass::get_instance()->type_object());
    _builtins->put(new PyString("dict"), DictKlass::get_instance()->type_object());
    _builtins->put(new PyString("isinstance"), new FunctionObject(isinstance));
    _builtins->put(new PyString("type"), new FunctionObject(type_of));
}

void Interpreter::run(CodeObject *codeObject) {
    _frame = new FrameObject(codeObject);

    //作为函数入口的那个模块，它的局部变量表里会设计__name__为__main__. 在程序开始，也就是创建第一个栈帧的时候才加入这个初始化
    _frame->locals()->put(new PyString("__name__"), new PyString("__main__"));

    while (_frame->has_more_codes()) {
        unsigned int opcode = _frame->get_op_code();
        PyObject *v, *w, *u;
        Block* b;
        FunctionObject* fo;
        ArrayList<PyObject*>* args = NULL;
        //大于等于90的字节码都带参数
        bool has_argument = (opcode & 0xFF) >= ByteCode::HAVE_ARGUMENT;
        int op_arg = -1;
        if (has_argument) {
            op_arg = _frame->get_op_arg();
        }

        switch (opcode) {
            case ByteCode::POP_TOP:
                POP();
                break;
            case ByteCode::LOAD_CONST:
                PUSH(_frame->consts()->get(op_arg));
                break;
            case ByteCode::PRINT_ITEM:
                v = POP();
                v->print();
                break;
            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;
            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                PUSH(w->add(v));
                break;
            case ByteCode::BINARY_MULTIPLY:
                v = POP();
                w = POP();
                PUSH(w->mul(v));
                break;
            case ByteCode::RETURN_VALUE:
                _ret_value = POP();
                if (_frame->is_first_frame()) {
                    return;
                }
                leave_frame();
                break;
            case ByteCode::COMPARE_OP://分支结构所需字节码 COMPARE_OP POP_JUMP_IF_FALSE JUMP_FORWARD
                w = POP();
                v = POP();
                switch (op_arg) {
                    case ByteCode::COMPARE::EQUAL:
                        PUSH(v->equal(w));
                        break;
                    case ByteCode::COMPARE::NOT_EQUAL:
                        PUSH(v->not_equal(w));
                        break;
                    case ByteCode::COMPARE::GREATER:
                        PUSH(v->greater(w));
                        break;
                    case ByteCode::COMPARE::LESS:
                        PUSH(v->less(w));
                        break;
                    case ByteCode::COMPARE::GREATER_EQUAL:
                        PUSH(v->ge(w));
                        break;
                    case ByteCode::COMPARE::LESS_EQUAL:
                        PUSH(v->le(w));
                        break;
                    case ByteCode::COMPARE::IS:
                        if (v == w) {
                            PUSH(PY_TRUE);
                        } else {
                            PUSH(PY_FALSE);
                        }
                        break;
                    case ByteCode::COMPARE::IS_NOT:
                        if (v == w) {
                            PUSH(PY_FALSE);
                        } else {
                            PUSH(PY_TRUE);
                        }
                        break;
                    case ByteCode::IN:
                        PUSH(w->contains(v));
                        break;
                    case ByteCode::NOT_IN:
                        if (w->contains(v) == Universe::PyTrue) {
                            PUSH(Universe::PyFalse);
                        } else {
                            PUSH(Universe::PyTrue);
                        }
                        break;
                    default:
                        printf("Error: Unrecognized compare op %d\n", op_arg);

                }
                break;
            case ByteCode::POP_JUMP_IF_FALSE:
                v = _frame->_stack->pop();
                if (v == Universe::PyFalse) {
                    _frame->set_pc(op_arg);
                }
                break;
            case ByteCode::JUMP_FORWARD: //例如if else 分支中跳过else就是通过这个指令，往前跳转，跳过else部分
                _frame->set_pc(_frame->get_pc() + op_arg);
                break;
            case ByteCode::STORE_NAME:
                _frame->locals()->put(_frame->names()->get(op_arg), POP());
                break;
                //LEGB原则，先去局部变量表里找，如果没有，再去全局变量表里找，还是没有就去builtin表里找
            case ByteCode::LOAD_NAME:
                v = _frame->names()->get(op_arg);
                w = _frame->locals()->get(v);
                if (w != Universe::PyNone) {
                    PUSH(w);
                    break;
                }
                w = _frame->globals()->get(v);
                if (w != Universe::PyNone) {
                    PUSH(w);
                    break;
                }
                w = _builtins->get(v);
                if (w != Universe::PyNone) {
                    PUSH(w);
                    break;
                }
                PUSH(Universe::PyNone);
                break;
            case ByteCode::SETUP_LOOP: // SETUP_LOOP和POP_BLOCK是为break语句准备的
                _frame->loop_stack()->add(new Block(opcode, _frame->get_pc()+op_arg, STACK_LEVEL()));
                break;
            case ByteCode::POP_BLOCK:
                b = _frame->loop_stack()->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                break;
            case ByteCode::JUMP_ABSOLUTE: //实现循环跳转的指令
                _frame->set_pc(op_arg);
                break;
            case ByteCode::BREAK_LOOP:
                b = _frame->loop_stack()->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                _frame->set_pc(b->_target);
                break;
            case ByteCode::MAKE_FUNCTION: //MAKE_FUNCTION 指令带的参数是，是一个整数代表该函数有多少个默认参数
                v = POP();
                fo = new FunctionObject(v);
                //在创建函数对象的时候，就把当前frame的globals传递给了FunctionObject。
                //从此不论这个函数被传递到哪里去执行，不论它执行上下文中的全局变量表的内容是什么，
                //这个函数一旦执行，它的全局变量表总是它定义时的那个
                fo->set_gloabls(_frame->globals());
                if (op_arg>0) {
                    args = new ArrayList<PyObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                fo->set_defalts(args);
                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                PUSH(fo);
                break;
            case ByteCode::CALL_FUNCTION:
                if (op_arg > 0) {
                    int na = op_arg & 0xff;
                    int nk = op_arg >> 8;
                    int arg_cnt = na + 2*nk;

                    args = new ArrayList<PyObject*>(arg_cnt);
                    while (arg_cnt--) {
                        args->set(arg_cnt, POP());
                    }
                }
                build_frame(POP(), args, op_arg);
                if (args != NULL) {
                    delete args;
                    args = NULL;
                }
                break;
            case ByteCode::LOAD_FAST:
                PUSH(_frame->fast_locals()->get(op_arg));
                break;
            case ByteCode::STORE_FAST:
                _frame->fast_locals()->set(op_arg, POP());
                break;
            case ByteCode::LOAD_GLOBAL:
                v = _frame->names()->get(op_arg);
                w = _frame->globals()->get(v);
                if (w != Universe::PyNone) {
                    PUSH(w);
                    break;
                }
                w = _builtins->get(v);
                if (w != Universe::PyNone) {
                    PUSH(w);
                    break;
                }
                PUSH(Universe::PyNone);
                break;
            case ByteCode::LOAD_ATTR://类的方法调研机制由这个指令支持，意为加载属性，可以理解为类的字段和方法都是类的属性
                v = POP();
                w = _frame->names()->get(op_arg);
                PUSH(v->getattr(w));
                break;
            case ByteCode::BUILD_TUPLE:
            case ByteCode::BUILD_LIST:
                v = new PyList();
                while (op_arg--) {
                    ((PyList*)v)->set(op_arg, POP());
                }
                PUSH(v);
                break;
            case ByteCode::BINARY_SUBSCR:
                v = POP();
                w = POP();
                PUSH(w->subscr(v));
                break;
            case ByteCode::STORE_SUBSCR:
                u = POP();
                v = POP();
                w = POP();
                v->store_subscr(u, w);
                break;
            case ByteCode::DELETE_SUBSCR:
                v = POP();
                w = POP();
                ((PyList*)w)->delete_subscr(v);
                break;
            case ByteCode::GET_ITER:
                v = POP();
                PUSH(v->iter());
                break;
            case ByteCode::FOR_ITER:
                v = TOP();
                w = v->getattr(StringTable::get_instance()->next_str);
                build_frame(w, NULL, op_arg);

                if (TOP() == NULL) {
                    _frame->set_pc(_frame->get_pc() + op_arg);
                    POP();
                }
                break;
            case ByteCode::BUILD_MAP://这条指令是带参数的，指示了这字典初始化时键值对个数。
                v = new PyDict();
                PUSH(v);
                break;
            case ByteCode::STORE_MAP:
                w = POP();
                v = POP();
                u = TOP();
                ((PyDict*)u)->put(w, v);
                break;
            case ByteCode::UNPACK_SEQUENCE:
                v = POP();
                while (op_arg--) {
                    PUSH(v->subscr(new PyInteger(op_arg)));
                }
                break;
            case ByteCode::STORE_DEREF://参数op_arg是变量表的序号，保存cell变量也就是闭包依赖的外部函数定义的变量
                _frame->closure()->set(op_arg, POP());
                break;
            case ByteCode::LOAD_DEREF:
                v = _frame->closure()->get(op_arg);
                if (v->klass() == CellKlass::getInstance()) {
                    v = ((CellObject*)v)->value();
                }
                PUSH(v);
                break;
            case ByteCode::LOAD_CLOSURE:
                //从closure表里取出来如果是空，就说明这个值不是局部变量，而是一个参数，
                //这种情况就先把这个cell变量从参数列表里取出来，再存入到closure中
                v = _frame->closure()->get(op_arg);
                if (v == NULL) {
                   v = _frame->get_cell_from_parameter(op_arg);
                   _frame->closure()->set(op_arg, v);
                }

                if (v->klass() == CellKlass::getInstance()) {
                    PUSH(v);
                } else {
                    PUSH(new CellObject(_frame->closure(), op_arg));
                }
                break;
            case ByteCode::MAKE_CLOSURE:
                v = POP();
                fo = new FunctionObject(v);
                fo->set_closure((PyList*) POP());
                fo->set_gloabls(_frame->globals());
                if (op_arg > 0) {
                    args = new ArrayList<PyObject*>();
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                fo->set_defalts(args);
                if (args != NULL) {
                    args = NULL;
                }
                PUSH(fo);
                break;
                //LOAD_LOCALS得到一个字典，里面记录了类A定义的各种方法和属性
            case ByteCode::LOAD_LOCALS:   //函数，lamuda表达式，和类定义的代码都会翻译成一个CodeObject,
                 PUSH(_frame->_locals);// LOAD_LOCALS指令是把当前栈帧的局部变量表加载到操作数栈
                break;                    //是整个类定义的的CodeObject的返回值
                //BUILD_CLASS要做的是用栈里的对象来创建新的Klass以及与它绑定的TypeObject。
                //栈里第一个参数是通过LOAD_LOCALS得到的一个字典，里面记录了类A定义的各种方法和属性,第二个参数是代表父类的元组
                //第三个是类名。类定义语句的作用是产生一个类对象TypeObject，并与类名绑定。执行类定义语句时，class定义里的那些代码都会被执行
            case ByteCode::BUILD_CLASS:
                v = POP();
                u = POP();
                w = POP();
                v = Klass::create_class(v, u, w);
                PUSH(v);
                break;
            default:
                printf("Error: Unrecognized byte code %d \n", opcode);
        }

    }

}

void Interpreter::build_frame(PyObject *pyObject, ArrayList<PyObject*>* args, int op_arg) {
    if (pyObject->klass() == NativeFunctionClass::get_instance()) {
        PUSH(((FunctionObject*)pyObject)->call(args));
    } else if (pyObject->klass() == FunctionKlass::get_instance()) {
        FrameObject* frameObject = new FrameObject((FunctionObject*) pyObject, args, op_arg);
        frameObject->set_sender(_frame);
        _frame = frameObject;
    } else if (pyObject->klass() == MethodKlass::get_instance()) {
        MethodObject* methodObject = (MethodObject*) pyObject;
        if (!args) {
            args = new ArrayList<PyObject*>();
        }
        args->insert(0, methodObject->owner());
        build_frame(methodObject->func(), args, op_arg+1);
    } else if (pyObject->klass() == TypeKlass::get_instance()) {
        PyObject* inst = ((PyTypeObject*) pyObject)->own_klass()->allocate_instance(pyObject, args);
        PUSH(inst);
    }
}

void Interpreter::destroy_frame() {
    FrameObject* temp = _frame;
    _frame = _frame->get_sender();
    delete temp;
}

void Interpreter::leave_frame() {
    destroy_frame();
    PUSH(_ret_value);
}