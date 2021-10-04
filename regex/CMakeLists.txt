cmake_minimum_required(VERSION 3.17)
project(regex)

set(CMAKE_CXX_STANDARD 14)

add_executable(regex main.cpp Regex.cpp Regex.h util.h ExprLexer.cpp ExprLexer.h MacroLexer.cpp MacroLexer.h util.cpp LexerBuffer.cpp NFA.cpp NFA.h DFA.cpp DFA.h)