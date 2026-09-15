#pragma once
enum class TokenType {
    // Literals
    Identifier, String, Number, 

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
    PlusEqual, MinusEqual,

    // Keywords
    And, Class, Else, False, Fn, For, If, Nil, Or, 
    Print, Return, Super, This, True, Var, While, 

    Eof
}; 
