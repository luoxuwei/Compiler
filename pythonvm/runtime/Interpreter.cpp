//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#include "Interpreter.h"
#include "../code/bytecode.h"
#include "../object/PyInteger.h"
#include "universe.h"
#define PUSH(x) _stack->add(x)
#define POP() _stack->pop()
#define STACK_LEVEL() _stack->size()

void Interpreter::run(CodeObject *codeObject) {
    _stack = new ArrayList<PyObject*>(codeObject->_stacks_size);
    _names = new Map<PyObject*, PyObject*>();
    _loop_stack = new ArrayList<Block*>();
    int pc = 0;
    int code_length = codeObject->_bytecodes->length();
    _consts = codeObject->_consts;

    while (pc < code_length) {
        unsigned int opcode = codeObject->_bytecodes->value()[pc++];
        PyObject *v, *w;
        Block* b;
        //大于等于90的字节码都带参数
        bool has_argument = (opcode & 0xFF) >= ByteCode::HAVE_ARGUMENT;
        int op_arg = -1;
        if (has_argument) {
            int byte1 = (codeObject->_bytecodes->value()[pc++] & 0xFF);
            op_arg = ((codeObject->_bytecodes->value()[pc++] & 0xFF) << 8) | byte1;
        }

        switch (opcode) {
            case ByteCode::LOAD_CONST:
                _stack->add(_consts->get(op_arg));
                break;
            case ByteCode::PRINT_ITEM:
                v = _stack->pop();
                v->print();
                break;
            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;
            case ByteCode::BINARY_ADD:
                v = _stack->pop();
                w = _stack->pop();
                _stack->add(v->add(w));
                break;
            case ByteCode::RETURN_VALUE:
                _stack->pop();
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
                v = _stack->pop();
                if (v == Universe::PyFalse) {
                    pc = op_arg;
                }
                break;
            case ByteCode::JUMP_FORWARD: //例如if else 分支中跳过else就是通过这个指令，往前跳转，跳过else部分
                pc += op_arg;
                break;
            case ByteCode::STORE_NAME:
                _names->put(codeObject->_names->get(op_arg), POP());
                break;
            case ByteCode::LOAD_NAME:
                PUSH(_names->get(codeObject->_names->get(op_arg)));
                break;
            case ByteCode::SETUP_LOOP: // SETUP_LOOP和POP_BLOCK是为break语句准备的
                _loop_stack->add(new Block(opcode, pc+op_arg, STACK_LEVEL()));
                break;
            case ByteCode::POP_BLOCK:
                b = _loop_stack->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                break;
            case ByteCode::JUMP_ABSOLUTE: //实现循环跳转的指令
                pc = op_arg;
                break;
            case ByteCode::BREAK_LOOP:
                b = _loop_stack->pop();
                while (STACK_LEVEL() > b->_level) {
                    POP();
                }
                pc = b->_target;
                break;
            default:
                printf("Error: Unrecognized byte code %d \n", opcode);
        }

    }

}