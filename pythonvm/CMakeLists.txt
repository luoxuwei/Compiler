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
        runtime/Interpreter.cpp
        code/bytecode.h
        runtime/universe.cpp
        util/map.cpp
        object/Klass.cpp
        runtime/FrameObject.cpp
        runtime/FunctionObject.cpp
        object/PyDict.cpp
        object/PyList.cpp
        object/Klass.cpp
        runtime/StringTable.cpp
        runtime/cellObject.cpp
        object/pytypeobject.cpp
        util/stack.cpp
        memory/heap.cpp
        memory/oopClosure.cpp)

add_executable(test main.cpp)

target_link_libraries(test pythonvm)