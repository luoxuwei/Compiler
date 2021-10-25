//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_CODETREEBUILDER_H
#define C_COMPILER_CODETREEBUILDER_H
#include <stack>
#include "ICodeNode.h"
#include "LRStateTableParser.h"
#include <deque>
#include <string>
#include <map>

using namespace std;
class CodeTreeBuilder {
private:
    static CodeTreeBuilder *instance;
    stack<ICodeNode *> codeNodeStack;
    LRStateTableParser *parser = NULL;
    TypeSystem *typeSystem = NULL;
    deque<void *> *valueStack = NULL;
    string * functionName;
    map<string, ICodeNode *> funcMap;
    CodeTreeBuilder();
    CodeTreeBuilder(CodeTreeBuilder *codeTreeBuilder) {}
    void assignSymbolToNode(ICodeNode *node, string &text);
public:
    static CodeTreeBuilder *getInstance();
    ICodeNode *buildCodeTree(int production, string text);
    ICodeNode *getCodeTreeRoot();
    ICodeNode * getFunctionNodeByName(string &name);
};


#endif //C_COMPILER_CODETREEBUILDER_H
