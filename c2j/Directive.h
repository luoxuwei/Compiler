//
// Created by 罗旭维 on 2021/10/31.
//

#ifndef C2J_DIRECTIVE_H
#define C2J_DIRECTIVE_H

class Directive {
public:
    enum VALUE {
        CLASS_PUBLIC,
        END_CLASS,
        SUPER,
        FIELD_PRIVATE_STATIC,
        METHOD_STATIC,
        METHOD_PUBLIC,
        METHOD_PUBBLIC_STATIC,
        END_METHOD,
        LIMIT_LOCALS,
        LIMIT_STACK,
        VAR,
        LINE,

        END,

    };

    static const char * toString(VALUE value) {
        const static char *directiveStr[] = {
                ".class public",
                ".end class",
                ".super",
                ".field private static",
                ".method static",
                ".method public",
                ".method public static",
                ".end method",
                ".limit locals",
                ".limit stack",
                ".var",
                ".line"

        };

        if (value >= VALUE::END) {
            return "";
        }

        return directiveStr[value];
    }
};


#endif //C2J_DIRECTIVE_H
