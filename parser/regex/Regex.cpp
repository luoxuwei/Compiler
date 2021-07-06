//
// Created by 罗旭维 on 2021/7/3.
//

#include "Regex.h"

void Regex::parse() {

}

Regex::Regex(const char *macroFilePath, char *regex) {
    exprLexer = new ExprLexer(macroFilePath, std::string(regex));
    exprLexer->advance();
}

/*
* term ->  character | [...] | [^...] | [character-charcter] | . | (expr)
*
*/
bool Regex::term() {
    if (exprLexer->matchToken(ExprLexer::Token::OPEN_PAREN)) {
        exprLexer->advance();
        expr();
        if (exprLexer->matchToken(ExprLexer::Token::CLOSE_PAREN)) {
            exprLexer->advance();
        }
        else {
            parseErr(Error::E_PAREN);
        }
        return true;
    }
    bool handle = character();
    if (!handle) {
        dot();
    }
    if (!handle) {
        charClass();
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

//term*
bool Regex::starClosure() {

    if (!exprLexer->matchToken(ExprLexer::Token::CLOSURE)) {
        return false;
    }

    NFA::State *start = nfa.newNfa();
    NFA::State *end = nfa.newNfa();
    start->next = nfa.startState();
    start->next2 = end;
    nfa.endState()->next = nfa.startState();
    nfa.endState()->next2 = end;
    nfa.setStartState(start);
    nfa.setEndState(end);
    exprLexer->advance();
    return true;
}

//term+
bool Regex::plusClosure() {

    if (!exprLexer->matchToken(ExprLexer::Token::PLUS_CLOSE)) {
        return false;
    }

    NFA::State *start = nfa.newNfa();
    NFA::State *end = nfa.newNfa();
    start->next = nfa.startState();
    nfa.endState()->next = nfa.startState();
    nfa.endState()->next2 = end;
    nfa.setStartState(start);
    nfa.setEndState(end);
    exprLexer->advance();
    return true;
}

//term?
bool Regex::optionClosure() {

    if (!exprLexer->matchToken(ExprLexer::Token::OPTIONAL)) {
        return false;
    }

    NFA::State *start = nfa.newNfa();
    NFA::State *end = nfa.newNfa();
    start->next = nfa.startState();
    start->next2 = end;
    nfa.endState()->next = end;
    nfa.setStartState(start);
    nfa.setEndState(end);
    exprLexer->advance();
    return true;
}

//factor -> term* | term+ | term?.
void Regex::factor() {
    bool handle = false;
    term();
    handle = starClosure();
    if (!handle) {
        handle = plusClosure();
    }
    if (!handle) {
        handle = optionClosure();
    }
}

//正则表达式连接操作 cat_expr -> factor factor .....
void Regex::cat_expr() {
    /*
     * cat_expr -> factor factor .....
     * 由于多个factor 前后结合就是一个cat_expr所以
     * cat_expr-> factor cat_expr
     */
    if (first_in_cat(exprLexer->token())) {
        factor();
    }
    NFA::State *start = nfa.startState();
    NFA::State *end = nfa.endState();
    while (first_in_cat(exprLexer->token())) {
        factor();
        end->next = nfa.startState();
        end = nfa.endState();
    }
    nfa.setStartState(start);
    nfa.setEndState(end);
}

/*
 * expr -> expr  “|”  cat_expr
 *         | cat_expr
 * cat_expr -> factor cat_expr
 *             | factor

 * factor -> term* | term+ | term?

 * term -> [string] | . | character | (expr) | [^string]
 *
 * */
void Regex::expr() {
    /*
    * expr 由一个或多个cat_expr 之间进行 OR 形成
    * 如果表达式只有一个cat_expr 那么expr 就等价于cat_expr
    * 如果表达式由多个cat_expr做或连接构成那么 expr-> cat_expr | cat_expr | ....
    * 由此得到expr的语法描述为:
    * expr -> expr OR cat_expr
    *         | cat_expr
    *
    */
    cat_expr();
    NFA::State *start1 = nfa.startState();
    NFA::State *end1 = nfa.endState();
    NFA::State *start2 = NULL, *end2 = NULL;
    while (exprLexer->matchToken(ExprLexer::Token::OR)) {
        exprLexer->advance();
        cat_expr();
        start2 = nfa.newNfa();
        end2 = nfa.newNfa();
        start2->next = start1;
        start2->next2 = nfa.startState();
        end1->next = end2;
        nfa.endState()->next = end2;
        start1 = start2;
        end1 = end2;
    }
    nfa.setStartState(start1);
    nfa.setEndState(end1);
}

//判断正则表达式的输入是否合法
bool Regex::first_in_cat(ExprLexer::Token token) {
    switch (token) {
        //正确的表达式不会以 ) $ 开头,如果遇到EOS表示正则表达式解析完毕，那么就不应该执行该函数
        case ExprLexer::Token::CLOSE_PAREN:
        case ExprLexer::Token::AT_EOL:
        case ExprLexer::Token::OR:
        case ExprLexer::Token::EOS:
            return false;
        case ExprLexer::Token::CLOSURE:
        case ExprLexer::Token::PLUS_CLOSE:
        case ExprLexer::Token::OPTIONAL:
            //*, +, ? 这几个符号应该放在表达式的末尾
            parseErr(Error::E_CLOSE);
            return false;
        case ExprLexer::Token::CCL_END:
            //表达式不应该以]开头
            parseErr(Error::E_BRACKET);
            return false;
        case ExprLexer::Token::AT_BOL:
            //^必须在表达式的最开始
            parseErr(Error::E_BOL);
            return false;
    }

    return true;
}

void Regex::printNfa() {
    nfa.printNfa();
}

/*
* 计算in集合中nfa节点所对应的ε闭包，
* 并将闭包的节点加入到in中
*/
void Regex::e_closure(std::set<NFA::State *> &in) {
    if (in.empty()) return;
    printf("ε-Closure( %s ) = ", stringFromNfa(in).c_str());

    std::stack<NFA::State*> stateStack;
    std::set<NFA::State *>::iterator iterator = in.begin();
    while (iterator != in.end()) {
        stateStack.push(*iterator);
        iterator++;
    }

    NFA::State *cur;
    while (!stateStack.empty()) {
        cur = stateStack.top();
        stateStack.pop();
        if (cur->edge == EPSILON && cur->next != NULL) {
            stateStack.push(cur->next);
            in.insert(cur->next);
        }
        if (cur->next2 != NULL) {
            stateStack.push(cur->next2);
            in.insert(cur->next2);
        }
    }
    printf("{ %s }", stringFromNfa(in).c_str());
}

std::string Regex::stringFromNfa(std::set<NFA::State *> set) {
    std::stack<NFA::State*> stateStack;
    std::set<NFA::State *>::iterator iterator = set.begin();
    std::set<NFA::State *>::iterator last = --set.end();
    std::string ret;

    while (iterator != set.end()) {
        ret.append(std::to_string((*iterator)->id));
        ret.append(",");
    }
}

void Regex::move(std::set<NFA::State *> &in, std::set<NFA::State *> &out, char c) {
    std::set<NFA::State *>::iterator iterator = in.begin();
    NFA::State *state;
    while (iterator != in.end()) {
        state = *iterator;
        iterator++;
        if (state->edge == c || (state->edge == CCL && state->inputSet.test(c))) {
            out.insert(state->next);
        }
    }
    printf("move({ %s }, ' %c ')= ", stringFromNfa(in).c_str(), c);
    printf("{ %s }", stringFromNfa(out).c_str());
}
