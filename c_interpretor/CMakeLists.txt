cmake_minimum_required(VERSION 3.17)
project(c_compiler)

set(CMAKE_CXX_STANDARD 14)


add_executable(c_interpretor main.cpp
        CLexer.cpp CLexer.h
        CTokenType.cpp CTokenType.h
        Production.cpp Production.h
        ProductionManager.cpp ProductionManager.h
        GrammarState.cpp GrammarState.h
        GrammarStateManager.cpp GrammarStateManager.h
        GrammarInitializer.cpp GrammarInitializer.h
        Symbols.cpp Symbols.h
        FirstSetBuilder.cpp FirstSetBuilder.h
        LRStateTableParser.cpp LRStateTableParser.h TypeLink.cpp TypeLink.h Symbol.cpp Symbol.h Specifier.cpp Specifier.h StructDefine.cpp StructDefine.h Declarator.cpp Declarator.h TypeSystem.cpp TypeSystem.h ICodeNode.cpp ICodeNode.h ICodeNodeImpl.cpp ICodeNodeImpl.h ICodeFactory.cpp ICodeFactory.h CodeTreeBuilder.cpp CodeTreeBuilder.h Executor.cpp Executor.h BaseExecutor.cpp BaseExecutor.h ExecutorFactory.cpp ExecutorFactory.h UnaryNodeExecutor.cpp UnaryNodeExecutor.h BinaryExecutor.cpp BinaryExecutor.h NoCommaExprExecutor.cpp NoCommaExprExecutor.h ExprExecutor.cpp ExprExecutor.h StatementExecutor.cpp StatementExecutor.h StatementListExecutor.cpp StatementListExecutor.h Intepretor.cpp Intepretor.h Value.cpp Value.h IValueSetter.cpp IValueSetter.h ArrayValueSetter.cpp ArrayValueSetter.h TestExecutor.cpp TestExecutor.h IfStatementExecutor.cpp IfStatementExecutor.h ElseStatementExecutor.cpp ElseStatementExecutor.h OptExprExecutor.cpp OptExprExecutor.h InitializerExecutor.cpp InitializerExecutor.h EndOptExecutor.cpp EndOptExecutor.h)