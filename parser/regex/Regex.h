//
// Created by 罗旭维 on 2021/7/3.
//

#ifndef REGEX_REGEX_H
#define REGEX_REGEX_H
#include <map>
#include <string>
#include "Lexer.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <algorithm>



class Regex {
private:
    std::map<std::string, std::string> macroMap;

public:
    Regex(const char * macroFilePath) {
        if (macroFilePath != NULL) {
            FILE* f = fopen(macroFilePath, "r");
            if (f == NULL) {
                assert(false);
            }
            struct stat statbuf;
            fstat(fileno(f), &statbuf);
            char buf[statbuf.st_size];
            fread(buf, statbuf.st_size, 1, f);
            Lexer lexer(buf, statbuf.st_size);
            processMacro(&lexer);
            fclose(f);
        }
    }

    void processMacro(Lexer *lexer) {
        while (lexer->lookahead(1) != EOF) {
            //去掉开头的空格或空行
            while (isspace(lexer->lookahead(1))) {
                lexer->advance();
            }

            std::string macroName;
            char c = lexer->lookahead(1);
            while (!isspace(c)) {
                macroName.push_back(c);
                lexer->advance();
                c = lexer->lookahead(1);
            }

            //忽略宏定义名称后面的空格
            while (isspace(lexer->lookahead(1))) {
                lexer->advance();
            }

            //获取宏定义的内容
            c = lexer->lookahead(1);
            std::string macroContent;
            while (!isspace(c)) {
                macroContent.push_back(c);
                lexer->advance();
                c = lexer->lookahead(1);
            }

            //将宏定义加入哈希表
            macroMap[macroName] = macroContent;

            while (isspace(lexer->lookahead(1))) {
                lexer->advance();
            }
        }
    }

    std::string& expandMacro(const std::string &name) {
        if (macroMap.find(name) == macroMap.end()) {
            assert(false);
        } else {
            std::string macro(macroMap[name]);
            macro.insert(0, "(");
            macro.append(")");
            return macro;
        }
    }

    void printMacs() {
        if (macroMap.empty()) {
            printf("There are no macros \n");
        }
        else {
            std::for_each(macroMap.begin(), macroMap.end(), [](std::map<std::string, std::string>::reference item) {
                printf("Macro name: %s Macro content: %s \n", item.first.c_str(), item.second.c_str());
            });
        }
    }

};


#endif //REGEX_REGEX_H
