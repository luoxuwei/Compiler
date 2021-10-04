//
// Created by 罗旭维 on 2021/7/2.
//

#include "LexerBuffer.h"

LexerBuffer::LexerBuffer(const char *filePath) {
    if (filePath != NULL) {
        FILE* f = fopen(filePath, "r");
        if (f == NULL) {
            assert(false);
        }
        struct stat statbuf;
        fstat(fileno(f), &statbuf);
        buf = new char[statbuf.st_size];
        buf_len = statbuf.st_size;
        fread(buf, statbuf.st_size, 1, f);
        fclose(f);
    } else {
        buf = NULL;
        buf_len = 0;
    }
}

LexerBuffer::LexerBuffer(const char *content, size_t len) {
    if (content == NULL || len <= 0) {
        return;
    }
    buf = new char[len];
    buf_len = len;
    memcpy(buf, content, len);
}

LexerBuffer::~LexerBuffer() {
    if (buf != NULL) {
        delete buf;
        buf = NULL;
    }
}

char * LexerBuffer::text() {
    int len = length();
    char *res = new char[len+1];
    memcpy(res, buf + cur_start, len);
    res[len] = 0;
    return res;
}

char * LexerBuffer::pre_text() {
    int len = pre_length();
    char *res = new char[len+1];
    memcpy(res, buf + pre_start, len);
    res[len] = 0;
    return res;
}

char LexerBuffer::advance() {
    if (next >= buf_len) {
        return EOF;
    }
    if (buf[next] == '\n') {
        lineno++;
    }
    return buf[next++];
}

int LexerBuffer::mark_start() {
    cur_end = cur_start = next;
    return cur_start;
}

int LexerBuffer::mark_end() {
    cur_end = next;
    return cur_end;
}

int LexerBuffer::to_mark() {
    next = cur_end;
    return next;
}

int LexerBuffer::mark_prev() {
    pre_start = cur_start;
    pre_lineno = lineno;
    return pre_start;
}

char LexerBuffer::lookahead(int n) {
    if (next + n -1 >= buf_len) {
        return EOF;
    }

    return buf[next + n -1];
}