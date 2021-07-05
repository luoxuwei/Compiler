//
// Created by 罗旭维 on 2021/7/5.
//

#ifndef REGEX_NFA_H
#define REGEX_NFA_H

#include <bitset>
#include <deque>
#include <stack>

#define EPSILON -1 //边对应的是ε
#define CCL -2; //边对应的是字符集
#define EMPTY -3; //该节点没有出去的边
#define ASCII_COUNT 127
#define NFA_MAX 256 //最多运行分配256个NFA节点

class NFA {
public:
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

    NFA();
    State* newNfa();
    void recycleNfa(State *state);
    State* startState() {return start;}
    State* endState() {return end;}
    void setStartState(State *state) {start = state;}
    void setEndState(State *state) {end = state;}

private:
    std::deque<State> states;
    std::stack<State*> stateStack;
    int nfaStates;
    State *start = NULL, *end = NULL;
};


#endif //REGEX_NFA_H
