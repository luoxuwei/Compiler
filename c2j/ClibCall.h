//
// Created by 罗旭维 on 2021/10/26.
//

#ifndef C_COMPILER_CLIBCALL_H
#define C_COMPILER_CLIBCALL_H
#include <set>
#include <string>
#include "Value.h"
#include "Symbol.h"

using namespace std;
class ClibCall {
private:
    set<string> apiSet;
    static  ClibCall *instance;
    ClibCall();
    Value *handlePrintfCall();
    Value *handleMallocCall();
    int calculateVarSize(Symbol *s);
    void generateJavaAssemblyForPrintf(string &s);
public:
    static ClibCall *getInstance();
    bool isAPICall(string &name);
    Value *invokeAPI(string &name);
    Value *handleSizeOfCall();
};


#endif //C_COMPILER_CLIBCALL_H
