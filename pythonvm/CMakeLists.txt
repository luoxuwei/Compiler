cmake_minimum_required(VERSION 3.17)
project(pythonvm)

set(CMAKE_CXX_STANDARD 14)

add_library(pythonvm SHARED
        util/arraylist.cpp
        util/BufferInputStream.h
        object/PyInteger.cpp
        object/PyObject.cpp
        object/PyString.cpp
        code/CodeObject.cpp
        code/binaryFileParser.cpp
        runtime/interpreter.cpp
        code/bytecode.h
        runtime/universe.cpp
        util/map.cpp)


add_executable(test main.cpp)

target_link_libraries(test pythonvm)