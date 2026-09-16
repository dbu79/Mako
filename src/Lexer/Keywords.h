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
};
