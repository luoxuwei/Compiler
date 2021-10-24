//
// Created by 罗旭维 on 2021/10/20.
//

#include "ICodeNodeImpl.h"
#include "ICodeFactory.h"
#include "Value.h"

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
    if (ICodeNode::TEXT == key) {
        name = (string *) value;
    }
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
    auto itr = attributeMap.find(ICodeNode::VALUE);

    if (itr != attributeMap.end()) {
        str.append("Node Value is ");
        str.append(((Value *)(itr->second))->toString());
    }
    itr = attributeMap.find(ICodeNode::TEXT);
    if (itr != attributeMap.end()) {
        str.append("\nNode Text is ");
        str.append(((string *)itr->second)->c_str());
    }
    str.append("\n Node Type is ");
    str.append(CTokenType::getSymbolStr(type));
    return str.c_str();
}

void ICodeNodeImpl::reverseChildren() {
    if (isChildrenReverse) {
        return;
    }

    std::reverse(children.begin(), children.end());
    isChildrenReverse = true;
}