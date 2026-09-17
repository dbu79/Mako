cmake_minimum_required(VERSION 3.15)
project(lox_interpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Mako
    main.cpp
    src/Error/ErrorReporter.cpp
    src/Lexer/Lexer.cpp
)

target_include_directories(Mako PRIVATE 
    src
)
