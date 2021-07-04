//
// Created by 罗旭维 on 2021/7/4.
//

#ifndef REGEX_MACROLEXER_H
#define REGEX_MACROLEXER_H
#include <string>
#include <map>

#include "LexerBuffer.h"
class MacroLexer {
private:
    std::map<std::string, std::string> macroMap;

public:
    MacroLexer(const char * macroFilePath);

    void processMacro(LexerBuffer &lexer);

    std::string expandMacro(const std::string &name);

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


#endif //REGEX_MACROLEXER_H
