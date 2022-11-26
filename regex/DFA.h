//
// Created by 罗旭维 on 2021/7/7.
//

#ifndef REGEX_DFA_H
#define REGEX_DFA_H
#include "NFA.h"
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#define ASCII_COUNT 127
//假定DFA状态机节点数不会超过254个
#define MAX_DFA_STATE_COUNT 254
class DFA {
public:
    const static int STATE_FAILURE = -1;
    typedef uint32_t state_t;

    struct State {
        std::set<NFA::State*> nfaStates; //dfa节点对应的nfa节点集合
        state_t id; //节点编号
        bool accepted = false; //是不是可接受状态
        bool hasNfaSet(std::set<NFA::State*> &set) {
            return nfaStates == set;
        }
    };

    struct StateGroup {
        state_t id;
        std::vector<State*> dfaGroup;
        std::vector<State*> tobeRemove;
        int size() {return dfaGroup.size();}
        State* get(std::size_t i) {
            if (i >= dfaGroup.size()) {
                return NULL;
            }
            return dfaGroup[i];
        }
        void add(State *state) {dfaGroup.push_back(state);}
        void print() {
            printf("Dfa Group num: %d it has dfa: ", id);
            std::vector<State*>::iterator iterator = dfaGroup.begin();
            while (iterator != dfaGroup.end()) {
                printf("%d ", (*iterator)->id);
                iterator++;
            }
            printf("\n");
        }

        void addToRemove(State *state) {
            tobeRemove.push_back(state);
        }

        void commitRemove() {
            std::vector<State*>::iterator iterator = tobeRemove.begin();
            std::vector<State*>::iterator last = dfaGroup.end();//!!!!!
            while (iterator != tobeRemove.end()) {
                last = std::remove(dfaGroup.begin(), dfaGroup.end(), *iterator);
                iterator++;
            }

            if (last != dfaGroup.end()) {
                dfaGroup.erase(last, dfaGroup.end());
            }
        }
    };

    DFA();
    State * getDfaFromNfaSet(std::set<NFA::State*> &nfaSet);
    void construct(NFA &nfa);
    void printDfa();

    StateGroup *createGroup() {
        StateGroup *ret;
        groups.resize(groups.size() + 1);
        ret = &groups.back();
        ret->id = groups.size();
        return ret;
    }

    void minimize();
    ~DFA();

private:
    std::deque<State> states;
    std::deque<StateGroup> groups;
    int dfaStateTransFormTable[MAX_DFA_STATE_COUNT][ASCII_COUNT+1];
    state_t (*minDfa)[ASCII_COUNT+1];
    std::vector<State *> dfaList;
    bool addNewGroup = false;
    StateGroup *newGroup;

    void printDfaState(State *state);
    State *isNfaStatesExistInDfa(std::set<NFA::State*> &set);
    bool isOnNumberClass(int from, int to);
    bool isOnDot(int from, int to);
    void doGroupSeperation();
    bool doGroupSeperationOnInput(StateGroup *group, State *first, State *next, char c);
    StateGroup *getContainingGroup(state_t state);
    void createMiniDfaTransTable();
    void printMiniDfaTable();
};


#endif //REGEX_DFA_H
