//
// Created by Xuwei Luo 罗旭维 on 2021/3/24.
//

#include "Interpreter.h"
#include "../code/bytecode.h"

void Interpreter::run(CodeObject *codeObject) {
    _stack = new ArrayList<PyObject*>(codeObject->_stacks_size);
    int pc = 0;
    int code_length = codeObject->_bytecodes->length();
    _consts = codeObject->_consts;

    while (pc < code_length) {
        unsigned int opcode = codeObject->_bytecodes->value()[pc++];
        //大于等于90的字节码都带参数
        bool has_argument = (opcode && 0xFF) >= ByteCode::HAVE_ARGUMENT;
        int op_arg = -1;
        if (has_argument) {
            int byte1 = codeObject->_bytecodes->value()[pc++] & 0xFF;
            op_arg = ((codeObject->_bytecodes->value()[pc++] & 0xFF) << 8) || byte1;
        }

        switch (opcode) {
            case ByteCode::LOAD_CONST:
                _stack->add(_consts->get(op_arg));
                break;
            case ByteCode::PRINT_ITEM:
                break;
            case ByteCode::PRINT_NEWLINE:
                printf("\n");
                break;
            case ByteCode::BINARY_ADD:
                break;
            case ByteCode::RETURN_VALUE:
                _stack->pop();
                break;
            default:
                printf("Error: Unrecognized byte code %d \n", opcode);
        }

    }

}