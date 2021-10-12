//
// Created by 罗旭维 on 2021/10/12.
//

#ifndef CPARSE_LRSTATETABLEPARSER_H
#define CPARSE_LRSTATETABLEPARSER_H
#include "CLexer.h"
#include "GrammarStateManager.h"
#include <string>
#include <stack>
#include <map>
#include <deque>
class LRStateTableParser {
private:
    CLexer *lexer;
    string text;
    CTokenType::Token lexerInput;
    const char *names[8] = {"t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7"};
    const char *attributeForParentNode = NULL;
    int curName = 0;
    stack<int> statusStack;
    deque<const char *> valueStack;
    stack<int> parseStack;
    GrammarStateManager::LRStateTable *lrStateTable;
    const char * new_name();
    void free_name(const char *s);
    void showCurrentStateInfo(int stateNum);
    int getAction(int currentState, CTokenType::Token currentInput);
public:
    LRStateTableParser(CLexer *);
    void parse();
};


#endif //CPARSE_LRSTATETABLEPARSER_H
