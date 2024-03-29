//
// Created by 罗旭维 on 2021/10/31.
//

#ifndef C2J_PROGRAMGENERATOR_H
#define C2J_PROGRAMGENERATOR_H
#include "CodeGenerator.h"

class ProgramGenerator : public CodeGenerator {
private:
    static ProgramGenerator *instance;
    ProgramGenerator() : CodeGenerator() {}

public:
    static ProgramGenerator *getInstance();
    void generateHeader();
    void finish();
    const char * getProgramName();
};


#endif //C2J_PROGRAMGENERATOR_H
