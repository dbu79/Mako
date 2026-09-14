#pragma once
enum class TokenType {
    // Literals
    Identifier, Integer, Float, 
    
    // Single Character Tokens
    LeftParen, RightParen,
    LeftBrace, RightBrace,
    Plus, Minus, 
    Star, Slash, 
    Comma, Dot,
    Colon, SemiColon,

    // One or two character tokens
    Bang, BangEqual, 
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,
    PlusPlus, MinusMinus,

    // Keywords
    And, Class, Else, False, Fn, For, If, Nil, Or, 
    Print, Return, Super, This, True, Var, While, 

    Eof
}; 
