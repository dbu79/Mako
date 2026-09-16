#pragma once
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

    // Keywords
    AND, CLASS, ELSE, FALSE, FN, FOR, IF, NIL, OR, 
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, BREAK,

    END_OF_FILE
}; 
