//
// Created by 罗旭维 on 2021/7/5.
//

#ifndef REGEX_NFA_H
#define REGEX_NFA_H

#include <bitset>
#include <deque>
#include <stack>
#include "LexerBuffer.h"
#include <set>
#include <iterator>

#define ASCII_COUNT 127
#define NFA_MAX 256 //最多运行分配256个NFA节点

class NFA {
public:
    const static int EPSILON = -1; //边对应的是ε
    const static int CCL = -2;     //边对应的是字符集
    const static int EMPTY = -3;   //该节点没有出去的边
    typedef uint32_t state_t;
    /*
     *位置锚定
     * */
    enum ANCHOR {
        NONE,
        START,//^：表示锚定行首
        END,  //$：表示锚定行尾
        BOTH
    };

    struct State {
        std::bitset<ASCII_COUNT> inputSet; //用来存储字符集类
        State* next;  //跳转的下一个状态，可以是空
        State* next2; //跳转的另一个状态，当状态含有两条ε边时，这个指针才有效
        ANCHOR anchor; //对应的正则表达式是否开头含有^, 或结尾含有$,  或两种情况都有
        state_t id; //节点编号
        bool visited = false; //节点是否被访问过，用于节点打印
        int edge; //记录转换边对应的输入，输入可以是空, ε，字符集(CCL),或空，也就是没有出去的边
        void clearState() {
            inputSet.reset();
            next = next2 = NULL;
            anchor = ANCHOR::NONE;
            id = -1;
        }

        void cloneNfa(State * state) {
            inputSet.reset();
            inputSet |= state->inputSet;
            anchor = state->anchor;
            edge = state->edge;
            next = state->next;
            next2 = state->next2;
        }
    };

    bool debug = true;

    NFA();
    State* newNfa();
    void recycleNfa(State *state);
    State* startState() {return start;}
    State* endState() {return end;}
    void setStartState(State *state) {start = state;}
    void setEndState(State *state) {end = state;}

    void printNfa();
    void match(std::string &str);
    /*
     * 计算in集合中nfa节点所对应的ε闭包，
     * 并将闭包的节点加入到in中
     * */
    void e_closure(std::set<State*> &in);

    /*
     * 求状态集合对应某个输入字符的转移集合
     * */
    void move(std::set<State*> &in, std::set<State*> &out, char c);

private:
    std::deque<State> states;
    std::stack<State*> stateStack;
    int nfaStates = 0;
    State *start = NULL, *end = NULL;

    void printNfa(State *state);
    void printNfaNode(State *state);
    void printCCL(State *state);

    std::string stringFromNfa(std::set<State*> &set);
    bool hasAcceptState(std::set<State*> &set);
};


#endif //REGEX_NFA_H
