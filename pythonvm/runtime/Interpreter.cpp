//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#include "Interpreter.h"
#include "../code/bytecode.h"
#include "../object/PyInteger.h"
#define PUSH(x) _stack->add(x)
#define POP() _stack->pop()

void Interpreter::run(CodeObject *codeObject) {
    _stack = new ArrayList<PyObject*>(codeObject->_stacks_size);
    int pc = 0;
    int code_length = codeObject->_bytecodes->length();
    _consts = codeObject->_consts;

    while (pc < code_length) {
        unsigned int opcode = codeObject->_bytecodes->value()[pc++];
        PyObject *v, *w;
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
            case ByteCode::COMPARE_OP:
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
                if (((PyInteger*) v)->value() == 0) {
                    pc = op_arg;
                }
                break;
            case ByteCode::JUMP_FORWARD:
                pc += op_arg;
                break;
            default:
                printf("Error: Unrecognized byte code %d \n", opcode);
        }

    }

}