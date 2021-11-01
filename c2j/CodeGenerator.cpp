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
    if (buffered) {
        bufferedContent.append(Directive::toString(directive));
        bufferedContent.append("\n");
        return;
    }
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, const char *operand) {
    if (buffered) {
        bufferedContent.append(Directive::toString(directive));
        bufferedContent.append(" ");
        bufferedContent.append(operand);
        bufferedContent.append("\n");
        return;
    }
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<" ";
    assemblyFile<<operand;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, int operand) {
    if (buffered) {
        bufferedContent.append(Directive::toString(directive));
        bufferedContent.append(" ");
        bufferedContent.append(to_string(operand));
        bufferedContent.append("\n");
        return;
    }
    assemblyFile<<Directive::toString(directive);
    assemblyFile<<" ";
    assemblyFile<<operand;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitDirective(Directive::VALUE directive, const char *operand1, const char *operand2) {
    if (buffered) {
        bufferedContent.append(Directive::toString(directive));
        bufferedContent.append(" ");
        bufferedContent.append(operand1);
        bufferedContent.append(" ");
        bufferedContent.append(operand2);
        bufferedContent.append("\n");
        return;
    }
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
    if (buffered) {
        bufferedContent.append(Directive::toString(directive));
        bufferedContent.append(" ");
        bufferedContent.append(operand1);
        bufferedContent.append(" ");
        bufferedContent.append(operand2);
        bufferedContent.append(" ");
        bufferedContent.append(operand3);
        bufferedContent.append("\n");
        return;
    }
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
    if (buffered) {
        bufferedContent.append("\t");
        bufferedContent.append(Instruction::toString(instruction));
        bufferedContent.append("\n");
        return;
    }
    assemblyFile<<"\t";
    assemblyFile<<Instruction::toString(instruction);
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emit(Instruction::VALUE instruction, const char *operand) {
    if (buffered) {
        bufferedContent.append("\t");
        bufferedContent.append(Instruction::toString(instruction));
        bufferedContent.append("\t");
        bufferedContent.append(operand);
        bufferedContent.append("\n");
        return;
    }
    assemblyFile<<"\t";
    assemblyFile<<Instruction::toString(instruction);
    assemblyFile<<"\t";
    assemblyFile<<operand;
    assemblyFile<<"\n";
    assemblyFile.flush();
    ++instructionCount;
}

void CodeGenerator::emitBlankLine() {
    if (buffered) {
        bufferedContent.append("\n");
        return;
    }
    assemblyFile<<"\n";
    assemblyFile.flush();
}

void CodeGenerator::finish() {
    assemblyFile.close();
}

void CodeGenerator::setNameAndDeclaration(string &name, string &declaration) {
    nameToDeclaration[name] = declaration;
}

string * CodeGenerator::getDeclarationByName(string &name) {
    auto iter = nameToDeclaration.find(name);
    if (iter != nameToDeclaration.end()) {
        return &iter->second;
    }
    return NULL;
}

void CodeGenerator::emitString(string &s) {
    if (buffered) {
        bufferedContent.append(s);
        bufferedContent.append("\n");
        return ;
    }
    assemblyFile<<s;
    assemblyFile.flush();
}

void CodeGenerator::emitBufferedContent() {
    if (!bufferedContent.empty()) {
        assemblyFile<<bufferedContent;
        assemblyFile.flush();
    }
}