//
// Created by 罗旭维 on 2021/10/31.
//

#ifndef C2J_CODEGENERATOR_H
#define C2J_CODEGENERATOR_H
#include <fstream>
#include <string>
#include <map>
#include "Directive.h"
#include "Instruction.h"

using namespace std;
class CodeGenerator {
private:
    static int instructionCount;
    ofstream assemblyFile;
    bool buffered = false;
    string bufferedContent;
    map<string, string> nameToDeclaration;

public:
    static const char *programName;
    CodeGenerator();
    void emitDirective(Directive::VALUE directive);
    void emitDirective(Directive::VALUE directive, const char *operand);
    void emitDirective(Directive::VALUE directive, int operand);
    void emitDirective(Directive::VALUE directive, const char *operand1, const char *operand2);
    void emitDirective(Directive::VALUE directive, const char *operand1, const char *operand2, const char *operand3);
    void emit(Instruction::VALUE instruction);
    void emit(Instruction::VALUE instruction, const char *operand);
    void emitBlankLine();
    void finish();
    void setNameAndDeclaration(string &name, string &declaration);
    string * getDeclarationByName(string &name);
    void setInstructionBuffered(bool isBuffer) {buffered = isBuffer;}
    void emitString(string &s);
    void emitBufferedContent();
};


#endif //C2J_CODEGENERATOR_H
