//
// Created by 罗旭维 on 2021/10/14.
//

#ifndef C_COMPILER_SPECIFIER_H
#define C_COMPILER_SPECIFIER_H
#include "StructDefine.h"

class Specifier {
public:
    //type
    const static int  NONE = -1;
    const static int  INT = 0;
    const static int  CHAR = 1;
    const static int  VOID = 2;
    const static int  STRUCTURE = 3;
    const static int  LABEL = 4;

    //storage
    const static int  FIXED = 0;
    const static int  REGISTER = 1;
    const static int  AUTO = 2;
    const static int  TYPEDEF = 3;
    const static int  CONSTANT = 4;

    const static int  NO_OCLASS = 0;  //如果内存类型是auto, 那么存储类型就是NO_OCLASS
    const static int  PUBLIC = 1;
    const static int  PRIVATE = 2;
    const static int  EXTERN = 3;
    const static int  COMMON = 4;

public:
    Specifier(){}
    int  basicType;
    void setType(int type) {
        basicType = type;
    }
    int getType() {
        return basicType;
    }

    int storageClass;
    void setStorageClass(int s) {
        storageClass = s;
    }
    int getStorageClass() {
        return storageClass;
    }

    int outputClass= NO_OCLASS;
    void setOutputClass(int c) {
        outputClass = c;
    }
    int getOutputClass() {
        return outputClass;
    }

    bool isLong = false;
    void setLong(bool l) {
        isLong = l;
    }

    bool getLong() {
        return isLong;
    }

    bool isSigned = false;
    void setSign(bool s) {
        isSigned = s;
    }

    bool isSign() {
        return isSigned;
    }

    bool isStatic = false;
    void setStatic(bool s) {
        isStatic = s;
    }

    bool is_Static() {
        return isStatic;
    }

    bool isExternal = false;
    void setExternal(bool e) {
        isExternal = e;
    }

    bool is_External() {
        return isExternal;
    }

    int  constantValue = 0;
    void setConstantVal(int v) {
        constantValue = v;
    }
    int getConstantVal() {
        return constantValue;
    }

    StructDefine *vStruct = NULL;
    void setStructObj(StructDefine *s) { vStruct = s;}
    StructDefine * getStructObj() {return vStruct;}
};


#endif //C_COMPILER_SPECIFIER_H
