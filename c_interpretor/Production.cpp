//
// Created by 罗旭维 on 2021/10/4.
//

#include "Production.h"
#include "ProductionManager.h"
#include <algorithm>
#include <cstdio>

Production::Production(int productionNum, CTokenType::Token left, int dot, vector<CTokenType::Token> &right): productionNum(productionNum), left(left), dotPos(dot), right(right) {
    if (dot >= right.size()) {
        dotPos = right.size();
    }
    lookAhead.push_back(CTokenType::Token::SEMI);
}

Production *Production::doForword() {
    Production *production = new Production(productionNum, left, dotPos + 1, right);
    production->lookAhead = vector<CTokenType::Token>(lookAhead);
    return production;
}

Production * Production::cloneSelf() {
    Production *production = new Production(productionNum, left, dotPos, right);
    production->lookAhead = vector<CTokenType::Token>(lookAhead);
    return production;
}

vector<CTokenType::Token> Production::computeFirstSetOfBetaAndC() {
    /*
     * 计算 First(β C)
     * 将β 和 C ,前后相连再计算他们的First集合，如果β 里面的每一项都是nullable的，那么
     * First(β C) 就是 First(β) 并上First(C), 由于C 必定是终结符的组合，所以First(C)等于C的第一个非终结符
     * 例如C = {+, * , EOI} , First(C) = {+}
     */
    vector<CTokenType::Token> set;
    vector<CTokenType::Token> firstSet;
    for (int i=dotPos+1; i<right.size(); i++) {
        set.push_back(right.at(i));
    }
    //beta 部分不为空,计算FIRST(B)作为该表达式的lookahad集合
    if (set.size() > 0) {
        for (int i=0; i<set.size(); i++) {
            auto l = ProductionManager::getInstance()->getFirstSetBuilder()->getFirstSet(set[i]);
            for (auto token : *l) {
                if (find(firstSet.begin(), firstSet.end(), token) == firstSet.end()) {
                    firstSet.push_back(token);
                }
            }

            if (!ProductionManager::getInstance()->getFirstSetBuilder()->isSymbolNullable(set[i])) {
                break;
            }

            if (i == l->size() -1) {
                //beta is composed by nulleable terms
                firstSet.insert(firstSet.end(), lookAhead.begin(), lookAhead.end());
            }
        }
    } else {
        //如果beta 部分为空的话，直接将上一个表达式的lookaHead集合作为该表达式的lookAhead集合
        firstSet.insert(firstSet.end(), lookAhead.begin(), lookAhead.end());
    }

    return firstSet;
}

void Production::addLookAheadSet(vector<CTokenType::Token> &list) {
//    for (auto token : list) {
//        if (find(lookAhead.begin(), lookAhead.end(), token) == lookAhead.end()) {
//            lookAhead.push_back(token);
//        }
//    }
    lookAhead = list;//TODO:注释部分为错误逻辑，导致生成跳转表错误，调试了很多次才发现。
}

CTokenType::Token Production::getDotSymbol() {
    if (dotPos >= right.size()) return CTokenType::Token::UNKNOWN_TOKEN;
    return right.at(dotPos);
}

bool Production::operator==(const Production &production) {
    /*
    * 判断两个表达式是否相同有两个条件，一是表达式相同，而是对应的Look ahead 集合也必须一致
    */
    return productionEquals(production) && lookAheadSetComparing(production) == 0;
}

bool Production::coverUp(const Production &production) {
    /*
    * 如果表达式相同，但是表达式1的look ahead 集合 覆盖了表达式2的look ahead 集合，
    * 那么表达式1 就覆盖 表达式2
    */
    return productionEquals(production) && lookAheadSetComparing(production) > 0;
}

bool Production::productionEquals(const Production &production) {
    return left == production.left && right == production.right && dotPos == production.dotPos;
}

int Production::lookAheadSetComparing(const Production &production) {
    if (lookAhead.size() > production.lookAhead.size()) {
        //looAhead 集合不但要比对方大，而且对方lookAhead的所有元素都在本方的lookAhead中，这样本方才算是覆盖对方
        for (int i = 0; i < production.lookAhead.size(); i++) {
            if (find(lookAhead.begin(), lookAhead.end(), production.lookAhead.at(i)) == lookAhead.end()) {
                return -1;
            }
        }
        return 1;
    }

    if (lookAhead.size() < production.lookAhead.size()) {
        return -1;
    }

    for (int i = 0; i < lookAhead.size(); i++) {
        if (lookAhead.at(i) != production.lookAhead.at(i)) {
            return -1;
        }
    }

    return 0;
}

void Production::print() {
    printf("%s -> ", CTokenType::getSymbolStr(left));
    bool printDot = false;
    for (int i=0; i<right.size(); i++) {
        if (i == dotPos) {
            printf(".");
            printDot = true;
        }
        printf("%s ", CTokenType::getSymbolStr(right.at(i)));
    }
    if (!printDot) {
        printf(".");
    }
    printf("\n");

    printf("look ahead set :{ ");
    for (auto token : lookAhead) {
        printf("%s ", CTokenType::getSymbolStr(token));
    }
    printf("}\n");
}

bool Production::canBeReduce() {
    return dotPos >= right.size();
}