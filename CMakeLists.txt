cmake_minimum_required(VERSION 3.30)
project(syntax_analyzer)

set(CMAKE_CXX_STANDARD 20)

add_executable(syntax_analyzer main.cpp
        Lexer.h
        Lexer.cpp
        AST.h
        BoundedDeque.h
        Parser.cpp
        Parser.h
)
