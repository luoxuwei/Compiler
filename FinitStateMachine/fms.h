//
// Created by 罗旭维 on 2021/7/2.
//

#ifndef FINITSTATEMACHINE_FMS_H
#define FINITSTATEMACHINE_FMS_H
#define ASCII_COUNT 128
#define STATE_COUNT 6
#define STATE_FAILURE -1

class FMS {
private:
    int fmsTable[STATE_COUNT][ASCII_COUNT];
    bool accept[STATE_COUNT] = {false, true, true, false, true, false};
    void initForNumber(int row, int val) {
        for (int i=0; i<=9; i++) {
            fmsTable[row]['0'+i] = val;
        }
    }
public:
    FMS() {
        for (int i=0; i<STATE_COUNT; i++) {
            for (int j=0; j<ASCII_COUNT; j++) {
                fmsTable[i][j] = STATE_FAILURE;
            }
        }
        fmsTable[0]['.'] = 3;
        fmsTable[1]['.'] = 2;
        fmsTable[1]['e'] = 5;
        fmsTable[2]['e'] = 5;
        initForNumber(0, 1);
        initForNumber(1, 1);
        initForNumber(3, 2);
        initForNumber(2, 2);
        initForNumber(5, 4);
        initForNumber(4, 4);
    }

    int next(int state, char c) {
        if (state == STATE_FAILURE || c >= ASCII_COUNT) {
            return STATE_FAILURE;
        }
        return fmsTable[state][c];
    }

    bool isAcceptSate(int state) {
        if (state != STATE_FAILURE) {
            return accept[state];
        }
        return false;
    }
};


#endif //FINITSTATEMACHINE_FMS_H
