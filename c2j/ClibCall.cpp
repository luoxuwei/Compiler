//
// Created by 罗旭维 on 2021/10/26.
//

#include "ClibCall.h"
#include "FunctionArgumentList.h"
#include "MemoryHeap.h"
#include "ProgramGenerator.h"
#include "Instruction.h"

ClibCall *ClibCall::instance = NULL;

ClibCall * ClibCall::getInstance() {
    if (instance == NULL) {
        instance = new ClibCall();
    }
    return instance;
}

bool ClibCall::isAPICall(string &name) {
    auto iter = apiSet.find(name);
    if (iter != apiSet.end()) return true;
    return false;
}

ClibCall::ClibCall() {
    apiSet.insert("printf");
    apiSet.insert("malloc");
    apiSet.insert("sizeof");
}

Value * ClibCall::invokeAPI(string &name) {
    if (name == "printf") {
        return handlePrintfCall();
    } else if (name == "malloc") {
        return handleMallocCall();
    } else if (name == "sizeof") {
        return handleSizeOfCall();
    }
    return NULL;
}

Value * ClibCall::handlePrintfCall() {
    vector<Value *> *list = FunctionArgumentList::getInstance()->getFuncArgList(false);
    string *argStr = list->at(0)->u.s;
    string formatStr;
    int i = 0;
    int argCount = 1;
    while (i < argStr->size()) {
        if (argStr->at(i) == '%' && i+1 < argStr->size() && argStr->at(i+1) == 'd') {
            generateJavaAssemblyForPrintf(formatStr);//把%d之前的字符串打印出来，然后通过printInteger打印数字
            formatStr.append(list->at(argCount)->toString());
            argCount++;
            i = i+2;
            printInteger();
        } else {
            formatStr.push_back(argStr->at(i));
            i++;
        }

    }
    printf(formatStr.c_str());
    printf("\n");
    string s("\n");
    generateJavaAssemblyForPrintf(s);
    return NULL;
}

Value * ClibCall::handleMallocCall() {
    vector<Value *> *list = FunctionArgumentList::getInstance()->getFuncArgList(false);
    int size = list->at(0)->u.i;
    int addr = 0;
    if (size > 0) {
        addr = MemoryHeap::allocMem(size);
    }
    return new Value(Value::POINTER, addr);
}

Value * ClibCall::handleSizeOfCall() {
    vector<Symbol *> *symList = FunctionArgumentList::getInstance()->getFuncArgSymbolList(false);
    return new Value(calculateVarSize(symList->at(0)));
}

int ClibCall::calculateVarSize(Symbol *s) {
    int size = 0;
    //arg list == NULL表示是基础数据类型
    if (s->getArgList() == NULL) {
        size = s->getByteSize();
    } else {
        Symbol *head = s->getArgList();
        while (head != NULL) {
            size += calculateVarSize(head);
            head = head->getNextSymbol();
        }
    }
    Declarator *declarator = s->getDeclarator(Declarator::ARRAY);
    if (declarator != NULL) {
        size = size * declarator->getElementNum();
    }
    return size;
}

void ClibCall::generateJavaAssemblyForPrintf(string &s) {
    ProgramGenerator::getInstance()->emit(Instruction::GETSTATIC, "java/lang/System/out Ljava/io/PrintStream;");
    string temp;
    temp.append("\"");
    temp.append(s);
    temp.append("\"");
    ProgramGenerator::getInstance()->emit(Instruction::LDC, temp.c_str());
    ProgramGenerator::getInstance()->emit(Instruction::INVOKEVIRTUAL, "java/io/PrintStream/print(Ljava/lang/String;)V");
}

void ClibCall::printInteger() {
    ProgramGenerator *programGenerator = ProgramGenerator::getInstance();
    programGenerator->emit(Instruction::ISTORE, "0");//把栈顶的值存到保存变量的数组的0位置，这样做是为了使得out对象处在要打印的数字的下面，
    // 下面通过getstaticv指令将out对象压入栈顶后会，从新把要打印的数值挪回栈顶
    programGenerator->emit(Instruction::GETSTATIC, "java/lang/System/out Ljava/io/PrintStream;");
    programGenerator->emit(Instruction::ILOAD, "0");
    programGenerator->emit(Instruction::INVOKEVIRTUAL, "java/io/PrintStream/print(I)V");
}