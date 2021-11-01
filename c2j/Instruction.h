//
// Created by 罗旭维 on 2021/10/31.
//

#ifndef C2J_INSTRUCTION_H
#define C2J_INSTRUCTION_H


class Instruction {
public:
    enum VALUE {
        LDC,
        GETSTATIC,
        SIPUSH,
        IADD,
        IMUL,
        INVOKEVIRTUAL,
        INVOKESTATIC,
        RETURN,
        IRETURN,
        ILOAD,
        ISTORE,
        END,

    };

    static const char * toString(VALUE value) {
        const static char *directiveStr[] = {
                "ldc",
                "getstatic",
                "sipush",
                "iadd",
                "imul",
                "invokevirtual",
                "invokestatic",
                "return",
                "ireturn",
                "iload",
                "istore"

        };

        if (value >= VALUE::END) {
            return "";
        }

        return directiveStr[value];
    }
};


#endif //C2J_INSTRUCTION_H
