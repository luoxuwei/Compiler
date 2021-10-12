cmake_minimum_required(VERSION 3.17)
project(CParse)

set(CMAKE_CXX_STANDARD 14)

add_executable(CParse main.cpp
        CLexer.cpp CLexer.h
        CTokenType.cpp CTokenType.h
        Production.cpp Production.h
        ProductionManager.cpp ProductionManager.h
        GrammarState.cpp GrammarState.h GrammarStateManager.cpp GrammarStateManager.h GrammarInitializer.cpp GrammarInitializer.h Symbols.cpp Symbols.h FirstSetBuilder.cpp FirstSetBuilder.h LRStateTableParser.cpp LRStateTableParser.h)