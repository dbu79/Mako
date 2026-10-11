#pragma once

#include <string>

enum class TokenType {
    // Literals
    IDENTIFIER, STRING, NUMBER, 

    // Single Character Tokens
    LEFTPAREN, RIGHTPAREN,
    LEFTBRACE, RIGHTBRACE,
    PLUS, MINUS, 
    STAR, SLASH, 
    COMMA, DOT,
    COLON, SEMICOLON,

    // One or Two Character Tokens
    BANG, BANGEQUAL, 
    EQUAL, EQUALEQUAL,
    GREATER, GREATEREQUAL,
    LESS, LESSEQUAL,
    PLUSPLUS, MINUSMINUS,
    PLUSEQUAL, MINUSEQUAL,
    LEFTARROW,

    // Keywords
    AND, CLASS, ELSE, FALSE, FN, FOR, IF, NIL, OR, 
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, BREAK,

    END_OF_FILE
}; 

inline std::string TokenToString(TokenType type) {
switch(type) {
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::STRING: return "STRING";
    case TokenType::NUMBER: return "NUMBER";

    case TokenType::LEFTPAREN: return "LEFTPAREN";
    case TokenType::RIGHTPAREN: return "RIGHTPAREN";
    case TokenType::LEFTBRACE: return "LEFTBRACE";
    case TokenType::RIGHTBRACE: return "RIGHTBRACE";

    case TokenType::PLUS: return "PLUS";
    case TokenType::MINUS: return "MINUS";
    case TokenType::STAR: return "STAR";
    case TokenType::SLASH: return "SLASH";
    case TokenType::COMMA: return "COMMA";
    case TokenType::DOT: return "DOT";
    case TokenType::COLON: return "COLON";
    case TokenType::SEMICOLON: return "SEMICOLON";

    case TokenType::BANG: return "BANG";
    case TokenType::BANGEQUAL: return "BANGEQUAL";
    case TokenType::EQUAL: return "EQUAL";
    case TokenType::EQUALEQUAL: return "EQUALEQUAL";

    case TokenType::GREATER: return "GREATER";
    case TokenType::GREATEREQUAL: return "GREATEREQUAL";
    case TokenType::LESS: return "LESS";
    case TokenType::LESSEQUAL: return "LESSEQUAL";
    case TokenType::LEFTARROW: return "LEFTARROW";

    case TokenType::PLUSPLUS: return "PLUSPLUS";
    case TokenType::MINUSMINUS: return "MINUSMINUS";
    case TokenType::PLUSEQUAL: return "PLUSEQUAL";
    case TokenType::MINUSEQUAL: return "MINUSEQUAL";

    case TokenType::AND: return "AND";
    case TokenType::CLASS: return "CLASS";
    case TokenType::ELSE: return "ELSE";
    case TokenType::FALSE: return "FALSE";
    case TokenType::FN: return "FN";
    case TokenType::FOR: return "FOR";
    case TokenType::IF: return "IF";
    case TokenType::NIL: return "NIL";
    case TokenType::OR: return "OR";
    case TokenType::PRINT: return "PRINT";
    case TokenType::RETURN: return "RETURN";
    case TokenType::SUPER: return "SUPER";
    case TokenType::THIS: return "THIS";
    case TokenType::TRUE: return "TRUE";
    case TokenType::VAR: return "VAR";
    case TokenType::WHILE: return "WHILE";
    case TokenType::BREAK: return "BREAK";

    case TokenType::END_OF_FILE: return "EOF";
    
    default: return "UNKNOWN";
    }
};
