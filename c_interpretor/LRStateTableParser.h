//
// Created by 罗旭维 on 2021/10/12.
//

#ifndef CPARSE_LRSTATETABLEPARSER_H
#define CPARSE_LRSTATETABLEPARSER_H
#include "CLexer.h"
#include "GrammarStateManager.h"
#include "TypeSystem.h"
#include <string>
#include <stack>
#include <map>
#include <deque>
class LRStateTableParser {
private:
    static LRStateTableParser *instance;
    CLexer *lexer;
    string text;
    string relOperatorText;
    CTokenType::Token lexerInput;
    int nestingLevel = 0;
    int enumValue = 0;
    const char *names[8] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7"};
    void *attributeForParentNode = NULL;
    TypeSystem *typeSystem;
    int curName = 0;
    stack<int> statusStack;
    deque<void *> valueStack;
    stack<int> parseStack;
    GrammarStateManager::LRStateTable *lrStateTable;
    LRStateTableParser();
    LRStateTableParser(LRStateTableParser &l) {}
    const char * new_name();
    void free_name(const char *s);
    void showCurrentStateInfo(int stateNum);
    int getAction(int currentState, CTokenType::Token currentInput);
    void takeActionForReduce(int productNum);
    void *takeActionForShift(CTokenType::Token token);
    void setFunctionSymbol(bool hasArgs);
    void doEnum();
    bool convSymToIntConst(Symbol *symbol, int val);
public:
    static LRStateTableParser *getInstance();
    void parse(CLexer *l);
    int getCurrentLevel() {return nestingLevel;};
    deque<void *> *getValueStack();
    string * getRelOpereatorText();
};


#endif //CPARSE_LRSTATETABLEPARSER_H
