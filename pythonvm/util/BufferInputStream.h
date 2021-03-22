//
// Created by Xuwei Luo 罗旭维 on 2021/3/22.
//

#ifndef PYTHONVM_BUFFERINPUTSTREAM_H
#define PYTHONVM_BUFFERINPUTSTREAM_H
#include <stdio.h>
#define BUFFER_LEN 256

class BufferInputStream {
private:
    FILE* fp;
    char szBuffer[BUFFER_LEN];
    unsigned short index;
public:
    BufferInputStream(char const * path) {
        fp = fopen(path, "rb");
        fread(szBuffer, sizeof(char)*BUFFER_LEN, 1, fp);
        index = 0;
    }

    char read() {
        if (index < BUFFER_LEN) {
            return szBuffer[index++];
        } else {
            fread(szBuffer, sizeof(char)*BUFFER_LEN, 1, fp);
            index = 0;
            return szBuffer[index++];
        }
    }

    int read_int() {
        int b1 = read() & 0xff;
        int b2 = read() & 0xff;
        int b3 = read() & 0xff;
        int b4 = read() & 0xff;
        return b4 << 24 | b3 << 16 | b2 << 8 | b1;
    }

    void unread() {
        index--;
    }

    void close() {
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }
    }
};

#endif //PYTHONVM_BUFFERINPUTSTREAM_H
