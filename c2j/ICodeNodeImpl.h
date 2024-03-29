//
// Created by 罗旭维 on 2021/10/20.
//

#ifndef C_COMPILER_ICODENODEIMPL_H
#define C_COMPILER_ICODENODEIMPL_H
#include "ICodeNode.h"
#include "CTokenType.h"
#include <map>
#include <vector>
#include <string>

using namespace std;
class ICodeNodeImpl : public ICodeNode {
private:
    map<ICodeKey, void *> attributeMap;
    CTokenType::Token type;
    vector<ICodeNode *> children;
    ICodeNode *parent = NULL;
    bool isChildrenReverse = false;
    string *name;
    string str;
public:
    ICodeNodeImpl(CTokenType::Token type);
    ICodeNode * getParent();
    ICodeNode * addChild(ICodeNode *node);
    vector<ICodeNode *> *getChildren();
    void setAttribute(ICodeKey key, void *value);
    void *getAttribute(ICodeKey key);
    ICodeNode *copy();
    const char *toString() override;
    void reverseChildren() override;
};


#endif //C_COMPILER_ICODENODEIMPL_H
