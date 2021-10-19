//
// Created by 罗旭维 on 2021/10/14.
//

#include "Symbol.h"

Symbol::Symbol(string name, int level) : name(name), level(level) {}

void Symbol::addDeclarator(TypeLink *type) {
    if (typeLinkBegin == NULL) {
        typeLinkBegin = type;
        typeLinkEnd = type;
    } else {
        type->setNextLink(typeLinkBegin);
        typeLinkBegin = type;
    }
}

void Symbol::addSpecifier(TypeLink *type) {
    if (typeLinkBegin == NULL) {
        typeLinkBegin = type;
        typeLinkEnd = type;
    } else {
        typeLinkEnd->setNextLink(type);
        typeLinkEnd = type;
    }
}

void Symbol::setNextSymbol(Symbol *symbol) {
    next = symbol;
}

Symbol * Symbol::getNextSymbol() {return next;}