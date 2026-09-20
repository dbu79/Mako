#pragma once

#include <unordered_map>
#include <string>

#include "TokenType.h"

const std::unordered_map<std::string, TokenType> Keywords = {
    { "and", TokenType::AND },
    { "class", TokenType::CLASS },
    { "else", TokenType::ELSE }, 
    { "false", TokenType::FALSE }, 
    { "for", TokenType::FOR },
    { "fn", TokenType::FN }, 
    { "if", TokenType::IF },
    { "nil", TokenType::NIL },
    { "or", TokenType::OR },
    { "print", TokenType::PRINT },
    { "return", TokenType::RETURN },
    { "super", TokenType::SUPER },
    { "this", TokenType::THIS },
    { "var", TokenType::VAR },
    { "true", TokenType::TRUE },
    { "while", TokenType::WHILE },
    { "break", TokenType::BREAK },
};
