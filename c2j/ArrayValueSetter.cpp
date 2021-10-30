//
// Created by 罗旭维 on 2021/10/22.
//

#include "ArrayValueSetter.h"
#include "Declarator.h"

ArrayValueSetter::ArrayValueSetter(Symbol *s, int i) : symbol(s), index(i) {

}

//对数组元素赋值时，要通过symbol得到declarator对象，调用declarator的接口对数组元素赋值
void ArrayValueSetter::setValue(Value *v) {
    Declarator *declarator = symbol->getDeclarator(Declarator::ARRAY);
    declarator->addElement(index, new Value(*v));
    printf("\nSet Value of %s to Array of name %s with index of %d", v->toString(), symbol->getName()->c_str(), index);
}

Symbol * ArrayValueSetter::getSymbol() {
    return symbol;
}