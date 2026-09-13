#pragma once

#include "TokenType.h"
#include <string_view>
#include <string>

struct Token {
    Token(TokenType type, std::string_view lexeme, std::size_t line)
        : Type(type), Lexeme(lexeme), Line(line){};

        TokenType Type; 
        std::string Lexeme;
        size_t Line;
};
