//
// Created by 罗旭维 on 2021/10/25.
//

#include "FunctDeclExecutor.h"
#include "GrammarInitializer.h"
#include "FunctionArgumentList.h"

void * FunctDeclExecutor::Execute(ICodeNode *root) {
    int production = (long) root->getAttribute(ICodeNode::PRODUCTION);
    Symbol *symbol = NULL, *args = NULL;
    currentNode = root;
    ICodeNode *n = NULL;
    string *name = NULL;
    string declaration, arg;
    switch (production) {
        case GrammarInitializer::NewName_LP_RP_TO_FunctDecl:
            //没有参数传递，只是把下一个节点的内容拷贝到当前节点上来，下一个节点就是NAME_TO_NewName产生的NewName节点，只是记录了变量名的字符串
            root->reverseChildren();
            n = root->getChildren()->at(0);
            name = (string *) n->getAttribute(ICodeNode::TEXT);
            if (name != NULL && *name != "main") {
                declaration = string(*name);
                declaration.append("()V");
                programGenerator->emitDirective(Directive::METHOD_PUBBLIC_STATIC, declaration.c_str());
                programGenerator->setNameAndDeclaration(*name, declaration);
            }
            copyChild(root, root->getChildren()->at(0));
            break;
        case GrammarInitializer::NewName_LP_VarList_RP_TO_FunctDecl:
            n = root->getChildren()->at(0);
            name = (string *) n->getAttribute(ICodeNode::TEXT);
            if (name != NULL && *name != "main") {
                declaration = string(*name);
                emitArgs(arg);
                declaration.append(arg);
                programGenerator->emitDirective(Directive::METHOD_PUBBLIC_STATIC, declaration.c_str());
                programGenerator->setNameAndDeclaration(*name, declaration);
            }
            symbol = (Symbol *) root->getAttribute(ICodeNode::SYMBOL);
            //获得参数列表
            args = symbol->getArgList();//拿到在函数调用节点（Unary_LP_RP_TO_Unary和Unary_LP_ARGS_RP_TO_Unary）执行时获得的参数值的列表，设置到函数声明的参数节点中
            initArgumentList(args);
            if (args == NULL || argList == NULL || argList->size() == 0) {
                //如果参数为空，那就是解析错误
                printf("\nExecute function with arg list but arg list is null\n");
                throw 0;
            }
            break;
    }

    return root;
}

void FunctDeclExecutor::initArgumentList(Symbol *args) {
    if (args == NULL) return;

    argList = FunctionArgumentList::getInstance()->getFuncArgList(true);

    Symbol *eachSym = args;
    int count = 0;
    while (eachSym != NULL) {
        IValueSetter *setter = (IValueSetter *) eachSym;
        /*
        * 将每个输入参数设置为对应值并加入符号表
        */
        setter->setValue(argList->at(count));
        count++;
        eachSym = eachSym->getNextSymbol();
    }
}

void FunctDeclExecutor::emitArgs(string &s) {
    vector<Symbol *> *argSymbolList = FunctionArgumentList::getInstance()->getFuncArgSymbolList(true);
    Symbol *symbol = NULL;
    s.append("(");
    for (int i = 0; i < argSymbolList->size(); ++i) {
        symbol = argSymbolList->at(i);
        string arg("");
        if (symbol->getDeclarator(Declarator::ARRAY) != NULL) {
            arg.append("[");
        }

        if (symbol->hasType(Specifier::INT)) {
            arg.append("I");
        }

        s.append(arg);

    }

    s.append(")V");
    programGenerator->emitString(s);
}
