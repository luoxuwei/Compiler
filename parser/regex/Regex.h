//
// Created by 罗旭维 on 2021/7/3.
//

#ifndef REGEX_REGEX_H
#define REGEX_REGEX_H
#include <map>
#include <vector>
#include <string>
#include "Lexer.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <algorithm>



class Regex {
private:
    std::map<std::string, std::string> macroMap;
    std::vector<std::string> regexExpr;

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
            processMacro(lexer);
            fclose(f);
        }
    }

    void processMacro(Lexer &lexer) {
        while (lexer.lookahead(1) != EOF) {
            //去掉开头的空格或空行
            while (isspace(lexer.lookahead(1))) {
                lexer.advance();
            }

            std::string macroName;
            char c = lexer.lookahead(1);
            while (!isspace(c)) {
                macroName.push_back(c);
                lexer.advance();
                c = lexer.lookahead(1);
            }

            //忽略宏定义名称后面的空格
            while (isspace(lexer.lookahead(1))) {
                lexer.advance();
            }

            //获取宏定义的内容
            c = lexer.lookahead(1);
            std::string macroContent;
            while (!isspace(c)) {
                macroContent.push_back(c);
                lexer.advance();
                c = lexer.lookahead(1);
            }

            //将宏定义加入哈希表
            macroMap[macroName] = macroContent;

            while (isspace(lexer.lookahead(1))) {
                lexer.advance();
            }
        }
    }

    std::string getMacro(const std::string &name) {
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


    void parse(const std::string& regex) {
        //扩展宏
        preProcessExpr(regex);

    }

    /*
     * 对正则表达式进行预处理，将表达式中的宏进行替换，例如
     * D*\.D 预处理后输出
     * [0-9]*\.[0-9]
     * 注意，宏是可以间套的，所以宏替换时要注意处理间套的情形
     */
    void preProcessExpr(const std::string& regex) {
        Lexer lexer(regex.c_str(), regex.length());
        while (lexer.lookahead(1) != EOF) {
            //去空格换行符等空白字符
            while (isspace(lexer.lookahead(1))) {
                lexer.advance();
            }

            std::string expr;
            bool inquoted = false;
            char c = lexer.advance();
            while (!isspace(c) && c != EOF) {
                if (c == '"') {
                    inquoted = !inquoted;
                }

                if (!inquoted && c == '{') {
                    expr.append(expandMacro(extracMacroNameFromInput(lexer)));
                } else {
                    expr.push_back(c);
                }

                c = lexer.advance();
            }

            //去空格换行符等空白字符
            while (isspace(lexer.lookahead(1))) {
                lexer.advance();
            }

            regexExpr.push_back(expr);
        }
    }

    std::string & extracMacroNameFromInput(Lexer &lexer) {
        char c = lexer.advance();
        std::string name;
        while (c != '}' && !isspace(c)) {
            name.push_back(c);
            c = lexer.advance();
        }

        if (c != '}') {
            assert(false);
        }
        return name;
    }

    std::string expandMacro(const std::string &name) {
        std::string content = getMacro(name);
        int begin = content.find('{');
        int end = std::string::npos;
        std::string temp;
        while (begin != std::string::npos) {
            end = content.find('}', begin+1);
            if (end == std::string::npos) {
                assert(false);//
            }
            bool inQuoted = checkInQuoted(content, begin, end);
            if (!inQuoted) {
                std::string curContent = getMacro(content.substr(begin + 1, end - begin -1));
                content.replace(begin, end - begin + 1, curContent);
                //当前遇到的宏展开，可能有嵌套，继续找'{'
                begin = content.find('{');
            } else {
                //end之前的内容已确认不是宏, 继续往后找'{'
                begin = content.find('{', end);
            }
        }
        return content;
    }

    bool checkInQuoted(std::string &macroContent, int curlyBracesBegin, int curlyBracesEnd) {
        int begin = macroContent.find('"');
        bool inQuoted = false;
        int end = std::string::npos;
        while (begin != std::string::npos) {
            end = macroContent.find('"', begin + 1);
            if (end == std::string::npos) {
                assert(false);
            }
            //"{}"
            if (curlyBracesBegin > begin && curlyBracesEnd < end) {
                inQuoted = true;
            } else if (curlyBracesBegin > begin && curlyBracesEnd > end) {
                //"{"} 大括号不匹配
                assert(false);
            } else if (curlyBracesBegin < begin && curlyBracesEnd < end) {
                //{"}" 大括号不匹配
                assert(false);
            }
            begin = macroContent.find('"', end+1);
        }
        return inQuoted;
    }

    void printExpr() {
        printf("regular expression after expanded: \n");
        if (regexExpr.size() > 0) {
            std::for_each(regexExpr.begin(), regexExpr.end(), [](std::vector<std::string>::value_type item) {
                printf(item.c_str());
            });
        }
    }

};


#endif //REGEX_REGEX_H
