//
// Created by 罗旭维 on 2021/10/14.
//

#include "Symbol.h"
#include "LRStateTableParser.h"

Symbol::Symbol(string name, int level) : name(name), level(level) {
    symbolScope = &LRStateTableParser::getInstance()->GLOBAL_SCOPE;
}

void Symbol::setValue(Value *v) {
    if (v != NULL) {
        printf("\nAssign Value of %s to Variable %s\n", v->toString(), name.c_str());
    }

    value = v;
}

void Symbol::addDeclarator(TypeLink *type) {
    if (typeLinkBegin == NULL) {
        typeLinkBegin = type;
        typeLinkEnd = type;
    } else {
        type->setNextLink(typeLinkBegin);
        typeLinkBegin = type;
    }
}

Declarator * Symbol::getDeclarator(int type) {
    TypeLink *begin = typeLinkBegin;
    while (begin != NULL && begin->getTypeObject() != NULL) {
        if (begin->isDeclarator) {
            Declarator *declarator = (Declarator *) begin->getTypeObject();
            if (declarator->getType() == type) {
                return declarator;
            }
        }

        begin = begin->toNext();
    }

    return NULL;
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

void Symbol::addScope(string *scope) {
    symbolScope = scope;
}

bool Symbol::operator==(const Symbol &s) {
    if (name == s.name && level == s.level && *symbolScope == *s.symbolScope) {
        return true;
    }

    return false;
}