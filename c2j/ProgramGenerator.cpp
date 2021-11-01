//
// Created by 罗旭维 on 2021/10/31.
//

#include "ProgramGenerator.h"

ProgramGenerator *ProgramGenerator::instance = NULL;
ProgramGenerator * ProgramGenerator::getInstance() {
    if (instance == NULL) {
        instance = new ProgramGenerator();
    }
    return instance;
}

const char * ProgramGenerator::getProgramName() {
    return programName;
}

void ProgramGenerator::generateHeader() {
    emitDirective(Directive::CLASS_PUBLIC, programName);
    emitDirective(Directive::SUPER, "java/lang/Object");
    emitBlankLine();
    emitDirective(Directive::METHOD_PUBBLIC_STATIC, "main([Ljava/lang/String;)V");
}

void ProgramGenerator::finish() {
    emit(Instruction::RETURN);
    emitDirective(Directive::END_METHOD);
    emitBufferedContent();
    emitDirective(Directive::END_CLASS);
    CodeGenerator::finish();
}