//
// Created by 罗旭维 on 2021/10/21.
//

#ifndef C_COMPILER_INTEPRETOR_H
#define C_COMPILER_INTEPRETOR_H
#include "Executor.h"

class Intepretor : public Executor {
private:
    static Intepretor *instance;
    Intepretor() {}
    Intepretor(Intepretor &intepretor) {}
public:
    static Intepretor *getInstance();
    void * Execute(ICodeNode *root) override;
};


#endif //C_COMPILER_INTEPRETOR_H
