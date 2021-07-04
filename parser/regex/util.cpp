//
// Created by 罗旭维 on 2021/7/4.
//

#include "util.h"

void parseErr(Error error) {
    const static char * errMsgs[11] =  {
            "Not enough memory for NFA",
            "Malformed regular expression",
            "Missing close parenthesis",
            "Too many regular expression or expression too long",
            "Missing [ in character class",
            "^ must be at the start of expression or after [",
            "+ ? or * must follow an expression or subexpression",
            "Newline in quoted string, use \\n to get newline into expression",
            "Missing ) in macro expansion",
            "Macro deoesn't exist",
            "Macro expansions nested too deeply"
    };
    exceptionmsg("%s", errMsgs[error])
}