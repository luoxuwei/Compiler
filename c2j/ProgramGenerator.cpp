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

void ProgramGenerator::generate() {
    emitDirective(Directive::CLASS_PUBLIC, programName);
    emitDirective(Directive::SUPER, "java/lang/Object");
    generateMainMethod();
}

void ProgramGenerator::generateMainMethod() {
    emitBlankLine();
    emitDirective(Directive::METHOD_PUBBLIC_STATIC, "main([Ljava/lang/String;)V");
}

void ProgramGenerator::finish() {
    emitDirective(Directive::END_METHOD);
    emitDirective(Directive::END_CLASS);
    CodeGenerator::finish();
}