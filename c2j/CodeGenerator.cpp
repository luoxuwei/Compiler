//
// Created by 罗旭维 on 2021/10/31.
//

#include "CodeGenerator.h"

int CodeGenerator::instructionCount = 0;
const char *CodeGenerator::programName = "CSourceToJava";

CodeGenerator::CodeGenerator() {
    string fileName(programName);
    fileName.append(".j");
    assemblyFile.open(fileName);
}

void CodeGenerator::emitDirective(Directive::VALUE directive) {
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, const char *operand) {
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<" ";
    assemblyFile<<operand;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, int operand) {
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<" ";
    assemblyFile<<operand;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, const char *operand1, const char *operand2) {
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<" ";
    assemblyFile<<operand1;
    assemblyFile<<" ";
    assemblyFile<<operand2;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, const char *operand1, const char *operand2, const char *operand3) {
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<" ";
    assemblyFile<<operand1;
    assemblyFile<<" ";
    assemblyFile<<operand2;
    assemblyFile<<" ";
    assemblyFile<<operand3;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emit(Instruction::VALUE instruction) {
    assemblyFile<<"\t";
    assemblyFile<<Instruction::toString(instruction);
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emit(Instruction::VALUE instruction, const char *operand) {
    assemblyFile<<"\t";
    assemblyFile<<Instruction::toString(instruction);
    assemblyFile<<"\t";
    assemblyFile<<operand;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitBlankLine() {
    assemblyFile<<"\n";
    assemblyFile.flush();
}

void CodeGenerator::finish() {
    assemblyFile.close();
}