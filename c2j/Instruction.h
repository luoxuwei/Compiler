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
        INVOKEVIRTUAL,
        RETURN,

        END,

    };

    static const char * toString(VALUE value) {
        const static char *directiveStr[] = {
                "ldc",
                "getstatic",
                "invokevirtual",
                "return"

        };

        if (value >= VALUE::END) {
            return "";
        }

        return directiveStr[value];
    }
};


#endif //C2J_INSTRUCTION_H
