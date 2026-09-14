
#include <array>
#include <regex>
#include <vector>
#include "Lexer.h"
#include "TokenType.h"

std::vector<Token> Lexer::ScanTokens(){
    while (!IsAtEnd()){
        m_Start = m_Current; 
        ScanToken();
    }
    Tokens_.emplace_back(TokenType::Eof, "", m_Line);
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
            Match('=') ? AddToken(TokenType::BangEqual) : AddToken(TokenType::Bang); 
            break; 
        case '>': 
            Match('=') ? AddToken(TokenType::GreaterEqual) : AddToken(TokenType::Greater);
            break;
        case '<':
            Match('=') ? AddToken(TokenType::LessEqual) : AddToken(TokenType::Less);
            break;

        case '+': 
            Match('+') ? AddToken(TokenType::PlusPlus) : AddToken(TokenType::Plus); 
            break; 
        case '-': 
            Match('-') ? AddToken(TokenType::MinusMinus) : AddToken(TokenType::Minus); 
            break; 
    };
};

void Lexer::AddToken(TokenType type){
    auto lexeme = Source.substr(m_Start, m_Start - m_Current);
    Tokens_.emplace_back(type, lexeme, m_Line);
};
    
void Lexer::Identifer(){};

bool Lexer::IsAtEnd() const {
    return m_Current >= Source.length();
}

bool Lexer::Match(char expected){};

bool Lexer::IsDigit() const {
    std::string nums[10] = {"0", "1", "2", "3", "4", "5", "6", "8", "9"};


};
bool Lexer::IsAlphaNumeric() const {};

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

