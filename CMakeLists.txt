cmake_minimum_required(VERSION 3.15)
project(lox_interpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Mako
    main.cpp
    src/Error/ErrorReporter.cpp
    src/Lexer/Lexer.cpp
    src/Parser/Parser.cpp
    src/Interpreter/Interpreter.cpp
)

target_include_directories(Mako PRIVATE 
    ${CMAKE_SOURCE_DIR}
)
