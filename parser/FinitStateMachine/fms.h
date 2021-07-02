//
// Created by 罗旭维 on 2021/7/2.
//

#ifndef FINITSTATEMACHINE_FMS_H
#define FINITSTATEMACHINE_FMS_H


class fms {
private:
public:
    int next(int state, char c);
    bool isAcceptSate(int state);
};


#endif //FINITSTATEMACHINE_FMS_H
