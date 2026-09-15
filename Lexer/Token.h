#pragma once

#include "TokenType.h"
#include <iterator>
#include <sstream>
#include <string_view>
#include <string>
#include <any>

struct Token {
    Token(TokenType type, std::string lexeme, std::any literal, std::size_t line)
        : Type(type), Lexeme(lexeme), Literal(literal),  Line(line){};

        TokenType Type; 
        std::string Lexeme;
        std::any Literal;
        size_t Line;

    std::string ToString() const {
        std::ostringstream oss;
        oss << "" << Lexeme << " " << Line;
        return oss.str();
    };
};
