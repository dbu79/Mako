#pragma once

#include "TokenType.h"
#include <sstream>
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
        
        oss << "Type: " << TokenToString(Type) << " ";
        oss << "Lexeme: " << Lexeme << " ";       

        if (Literal.has_value()){
            oss << "Literal: ";
            if (Literal.type() == typeid(std::string)){
                oss << std::any_cast<std::string>(Literal) << " ";
            } else if (Literal.type() == typeid(int)){
                oss << std::any_cast<int>(Literal) << " ";
            } else if (Literal.type() == typeid(double)){
                oss << std::any_cast<double>(Literal) << " ";
            } else {
                oss << "Unknown ";
            }
        }
        oss << "Line: " << Line;
        return oss.str();
    };
};
