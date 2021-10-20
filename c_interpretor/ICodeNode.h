//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_ICODENODE_H
#define C_COMPILER_ICODENODE_H
#include <vector>

using namespace std;
//每一个非终结符(能够处于语法推导表达式左边的符号)对应一个ICodeNode结点
class ICodeNode {
public:
    enum ICodeKey {
        TokenType, //当前解析对象的标签，就是Token
        VALUE, //是对应对象解析后的值，例如int a, a的值是1，Value就是1。如果当前解析的是数字，value就是数字的整形值
        SYMBOL, //符号对象
        PRODUCTION, //要推导的非终结符对应的表达式
        TEXT //如果当前结点对应的是一个变量，TEXT就是变量名.如果当前结点解析的是数字就是该数字的字符串
    };
    virtual ICodeNode * getParent() = 0;
    virtual ICodeNode * addChild(ICodeNode *node) = 0;
    virtual vector<ICodeNode *> *getChildren() = 0;
    virtual void setAttribute(ICodeKey key, void *value) = 0;
    virtual void *getAttribute(ICodeKey key) = 0;
    virtual ICodeNode *copy() = 0;
    virtual const char *toString() = 0;
};


#endif //C_COMPILER_ICODENODE_H
