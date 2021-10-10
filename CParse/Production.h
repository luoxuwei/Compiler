//
// Created by 罗旭维 on 2021/10/4.
//

#ifndef CPARSE_PRODUCTION_H
#define CPARSE_PRODUCTION_H
#include <vector>
#include "CTokenType.h"
using namespace std;
//表示语法推导表达式的类
class Production {
private:
    int dotPos = 0;//求闭包和状态跳转时用的标识
    CTokenType::Token left = CTokenType::Token::UNKNOWN_TOKEN;//语法推导表达式左边的非终结符
    vector<CTokenType::Token> &right;//语法推导表达式右边的符号列表
    vector<CTokenType::Token> lookAhead;
public:
    Production(CTokenType::Token left, int dot, vector<CTokenType::Token> &right);
    Production *doForword();//分区生成新的状态节点时，将位置标识往前以一步
    CTokenType::Token getLeft() {return left;};
    vector<CTokenType::Token> & getRight() {return right;};
    int getDotPosition() {return dotPos;};
    CTokenType::Token getDotSymbol();
    bool operator==(const Production & production);
    void print();
    Production * cloneSelf();
    vector<CTokenType::Token> computeFirstSetOfBetaAndC();
    void addLookAheadSet(vector<CTokenType::Token> &list);
    bool productionEquals(const Production &production);
    int lookAheadSetComparing(const Production &production);
    bool coverUp(const Production &production);
};


#endif //CPARSE_PRODUCTION_H
