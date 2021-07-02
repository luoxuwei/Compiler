cmake_minimum_required(VERSION 3.17)
project(FinitStateMachine)

set(CMAKE_CXX_STANDARD 14)

add_executable(FinitStateMachine main.cpp Lexer.cpp Lexer.h fms.cpp fms.h)