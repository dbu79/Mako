#include <vector>
#include <any>
#include "Lexer.h"
#include "TokenType.h"

std::vector<Token> Lexer::ScanTokens(){
    while (!IsAtEnd()){
        m_Start = m_Current; 
        ScanToken();
    }
    Tokens_.emplace_back(TokenType::Eof, "", nullptr, m_Line);
    return Tokens_;
};

void Lexer::ScanToken(){
    // view and call AddToken
    char c {Advance()};
    switch (c){
        // Single Char Tokens
        case '(': AddToken(TokenType::LeftParen); break; 
        case ')': AddToken(TokenType::RightParen); break; 
        case '{': AddToken(TokenType::LeftBrace); break; 
        case '}': AddToken(TokenType::RightBrace); break; 
        case '*': AddToken(TokenType::Star); break; 
        case ',': AddToken(TokenType::Comma); break; 
        case '.': AddToken(TokenType::Dot); break; 
        case ':': AddToken(TokenType::Colon); break; 
        case ';': AddToken(TokenType::SemiColon); break; 
        case '/': 
        // Handle Comments
            if (Match('/')){
                while(Peek() != '\n' && !IsAtEnd()) Advance();
            } else {
                AddToken(TokenType::Slash);
            } break;
        
        // Multi Char Tokens
        case '!': 
            AddToken(Match('=') ? TokenType::BangEqual : TokenType::Bang); 
            break; 
        case '>': 
            AddToken(Match('=') ? TokenType::GreaterEqual : TokenType::Greater);
            break;
        case '<':
            AddToken(Match('=') ? TokenType::LessEqual : TokenType::Less);
            break;
        case '+': 
            AddToken(Match('+') ? TokenType::PlusPlus : (Match('=') ? TokenType::PlusEqual : TokenType::Plus)); 
            break; 
        case '-': 
            AddToken(Match('-') ? TokenType::MinusMinus : (Match('=') ? TokenType::MinusEqual : TokenType::Minus)); 
            break; 

        // Whitespace
        case ' ': 
            [[fallthrough]];
        case '\r': 
            [[fallthrough]];
        case '\t':
            break;
        case '\n': 
            m_Line++;
            break;
        
        // Strings
        case '"':
            
        // Default case
    };
};

void Lexer::AddToken(TokenType type, std::any literal){
    auto lexeme = Source.substr(m_Start, m_Current - m_Start);
    Tokens_.emplace_back(type, lexeme, literal, m_Line);
};

void Lexer::AddToken(TokenType type){
    AddToken(type, nullptr);
}
    
void Lexer::Identifer(){
    while (IsAlphaNumeric(Peek())) Advance();

    AddToken(TokenType::Identifier);
};


void Lexer::Number(){
    while (IsDigit(Peek())) Advance();

    if (Peek() == '.' && IsDigit(PeekNext())){
        Advance();
        while (IsDigit(Peek())) Advance();
    };

    AddToken(TokenType::Number);
};

void Lexer::String(){
    while (Peek() != '"' && !IsAtEnd()){
        if (Peek() == '\n') {
            m_Line++;
        };
        Advance();
    }

    if (IsAtEnd()){
        // Error 
    }

    Advance();

    std::string value { Source.substr(m_Start + 1, (m_Current - m_Start) - 2) };
    AddToken(TokenType::String, value);

};

bool Lexer::IsAtEnd() const {
    return m_Current >= Source.length();
}

bool Lexer::Match(char expected){
    if (IsAtEnd()){
        return false;
    };
    if (Source[m_Current] != expected){
        return false;
    };
    m_Current++;
    return true;
};

bool Lexer::IsDigit(char c) const {
    if (c >= '0' && c <= '9') return true;
    return false;
};

bool Lexer::IsAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
};

bool Lexer::IsAlphaNumeric(char c) const {
    return IsDigit(c) || IsAlpha(c);
};

char Lexer::Advance(){
    m_Current++;
    return Source[m_Current - 1];
};

char Lexer::Peek(){
    if (IsAtEnd()){
        return '\0';
    } 
    return Source[m_Current];
};

char Lexer::PeekNext(){
    if (IsAtEnd()){
        return 0;
    } 
    return Source[m_Current + 1];
};

