//
// Created by 罗旭维 on 2021/10/20.
//

#include "ICodeNodeImpl.h"
#include "ICodeFactory.h"

ICodeNodeImpl::ICodeNodeImpl(CTokenType::Token type): type(type) {
    attributeMap[ICodeKey::TokenType] = (void *) type;
}

ICodeNode * ICodeNodeImpl::addChild(ICodeNode *node) {
    if (node != NULL) {
        children.push_back(node);
        ((ICodeNodeImpl *) node)->parent = this;
    }
    return node;
}

ICodeNode * ICodeNodeImpl::getParent() {return parent;}

vector<ICodeNode *> * ICodeNodeImpl::getChildren() {
    return &children;
}

void ICodeNodeImpl::setAttribute(ICodeKey key, void *value) {
    attributeMap[key] = value;
}

void * ICodeNodeImpl::getAttribute(ICodeKey key) {
    auto iter = attributeMap.find(key);
    if (iter != attributeMap.end()) {
        return attributeMap[key];
    }
    return NULL;
}

ICodeNode * ICodeNodeImpl::copy() {
    ICodeNodeImpl *copy = (ICodeNodeImpl *) ICodeFactory::createICodeNode(type);
    for (auto entry : attributeMap) {
        copy->setAttribute(entry.first, entry.second);
    }
    return copy;
}

const char * ICodeNodeImpl::toString() {
    return CTokenType::getSymbolStr(type);
}

void ICodeNodeImpl::reverseChildren() {
    if (isChildrenReverse) {
        return;
    }

    std::reverse(children.begin(), children.end());
    isChildrenReverse = true;
}