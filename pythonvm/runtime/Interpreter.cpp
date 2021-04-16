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

#define PUSH(x) _frame->_stack->add(x)
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
}

void Interpreter::run(CodeObject *codeObject) {
    _frame = new FrameObject(codeObject);

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
                    args = new ArrayList<PyObject*>(op_arg);
                    while (op_arg--) {
                        args->set(op_arg, POP());
                    }
                }
                build_frame(POP(), args);
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
                build_frame(w, NULL);

                if (TOP() == NULL) {
                    _frame->set_pc(_frame->get_pc() + op_arg);
                    POP();
                }
                break;
            default:
                printf("Error: Unrecognized byte code %d \n", opcode);
        }

    }

}

void Interpreter::build_frame(PyObject *pyObject, ArrayList<PyObject*>* args) {
    if (pyObject->klass() == NativeFunctionClass::get_instance()) {
        PUSH(((FunctionObject*)pyObject)->call(args));
    } else if (pyObject->klass() == FunctionKlass::get_instance()) {
        FrameObject* frameObject = new FrameObject((FunctionObject*) pyObject, args);
        frameObject->set_sender(_frame);
        _frame = frameObject;
    } else if (pyObject->klass() == MethodKlass::get_instance()) {
        MethodObject* methodObject = (MethodObject*) pyObject;
        if (!args) {
            args = new ArrayList<PyObject*>();
        }
        args->insert(0, methodObject->owner());
        build_frame(methodObject->func(), args);
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