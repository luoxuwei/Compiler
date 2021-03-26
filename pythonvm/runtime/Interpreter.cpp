//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#include "Interpreter.h"
#include "../code/bytecode.h"
#include "../object/PyInteger.h"
#include "universe.h"
#define PUSH(x) _frame->_stack->add(x)
#define POP() _frame->_stack->pop()
#define STACK_LEVEL() _frame->_stack->size()

void Interpreter::run(CodeObject *codeObject) {
    _frame = new FrameObject(codeObject);
    while (_frame->has_more_codes()) {
        unsigned int opcode = _frame->get_op_code();
        PyObject *v, *w;
        Block* b;
        //大于等于90的字节码都带参数
        bool has_argument = (opcode & 0xFF) >= ByteCode::HAVE_ARGUMENT;
        int op_arg = -1;
        if (has_argument) {
            op_arg = _frame->get_op_arg();
        }

        switch (opcode) {
            case ByteCode::LOAD_CONST:
                _frame->stack()->add(_frame->consts()->get(op_arg));
                break;
            case ByteCode::PRINT_ITEM:
                v = _frame->stack()->pop();
                v->print();
                break;
            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;
            case ByteCode::BINARY_ADD:
                v = POP();
                w = POP();
                _frame->stack()->add(v->add(w));
                break;
            case ByteCode::RETURN_VALUE:
                _frame->stack()->pop();
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
            case ByteCode::LOAD_NAME:
                PUSH(_frame->locals()->get(_frame->names()->get(op_arg)));
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
            default:
                printf("Error: Unrecognized byte code %d \n", opcode);
        }

    }

}