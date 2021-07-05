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

//字符集类的正则表达式,如:[abcd] 和字符集取反[^...]
bool Regex::charClass() {
    if (!exprLexer->matchToken(ExprLexer::Token::CCL_START)) {
        return false;
    }
    exprLexer->advance();
    NFA::State *start = nfa.newNfa();
    nfa.setStartState(start);
    NFA::State *end = nfa.newNfa();
    nfa.setEndState(end);
    start->next = end;
    start->edge = CCL;

    bool complement = false;
    if (exprLexer->matchToken(ExprLexer::Token::AT_BOL)) {
        complement = true;
    }

    if (!exprLexer->matchToken(ExprLexer::Token::CCL_END)) {
        dodash(start);
    }

    if (!exprLexer->matchToken(ExprLexer::Token::CCL_END)) {
        parseErr(Error::E_BADEXPR);
    }

    if (complement) {
        start->inputSet.flip();
    }
    
    exprLexer->advance();
    return true;
}

void Regex::dodash(NFA::State *state) {
    int first = 0;
    while (!exprLexer->matchToken(ExprLexer::Token::EOS) &&
            !exprLexer->matchToken(ExprLexer::Token::CCL_END)) {
        if (!exprLexer->matchToken(ExprLexer::Token::DASH)) {
            first = exprLexer->getLexeme();
            state->inputSet.set(first);
        } else {
            exprLexer->advance();//越过 -
            for (int i = first; i <= exprLexer->getLexeme(); i++) {
                state->inputSet.set(i);
            }
        }
        exprLexer->advance();
    }
}

