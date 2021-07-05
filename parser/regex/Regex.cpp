//
// Created by 罗旭维 on 2021/7/3.
//

#include "Regex.h"

void Regex::parse() {

}

Regex::Regex(const char *macroFilePath, char *regex) {
    exprLexer = new ExprLexer(macroFilePath, std::string(regex));
}

bool Regex::term() {
    bool handle = character();
    if (!handle) {
        dot();
    }
}

bool Regex::character() {
    if (!exprLexer->matchToken(ExprLexer::Token::L)) {
        return false;
    }

    NFA::State *start = nfa.newNfa();
    nfa.setStartState(start);
    NFA::State *end = nfa.newNfa();
    nfa.setEndState(end);
    start->next = end;
    start->edge = exprLexer->getLexeme();
    exprLexer->advance();
    return true;
}

//正则表达式 “.” 匹配除\n \r 外的任意单字符,
bool Regex::dot() {
    if (!exprLexer->matchToken(ExprLexer::Token::ANY)) {
        return false;
    }

    NFA::State *start = nfa.newNfa();
    nfa.setStartState(start);
    NFA::State *end = nfa.newNfa();
    nfa.setEndState(end);
    start->next = end;
    start->edge = CCL;
    start->inputSet.set('\n');
    start->inputSet.set('\r');
    start->inputSet.flip();
    exprLexer->advance();
}
