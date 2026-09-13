
#include <vector>
#include "Lexer.h"
#include "TokenType.h"

std::vector<Token> Lexer::ScanTokens(){
    while (!IsAtEnd()){
        m_Start = m_Current; 
        ScanToken();
    }
    Tokens.emplace_back(TokenType::Eof, "", m_Line);
    return Tokens;
};

void Lexer::ScanToken(){
    // view and call AddToken
    char c {Advance()};
    switch (c){
        case '(': AddToken(TokenType::LeftParen); break; 
        case ')': AddToken(TokenType::RightParen); break; 
        case '{': AddToken(TokenType::LeftBrace); break; 
        case '}': AddToken(TokenType::RightBrace); break; 
        case '+': AddToken(TokenType::Plus); break; 
        case '-': AddToken(TokenType::Minus); break; 
        case '*': AddToken(TokenType::Star); break; 
        case '/': AddToken(TokenType::Slash); break; 
        case ',': AddToken(TokenType::Comma); break; 
        case '.': AddToken(TokenType::Dot); break; 
        case '!': AddToken(TokenType::Bang); break; 
        case ':': AddToken(TokenType::Colon); break; 
        case ';': AddToken(TokenType::SemiColon); break; 
    };
};

void Lexer::AddToken(TokenType type){
    Tokens_.push_back(new Token(type, Source[m_Current], m_Line))
};
    
void Lexer::Identifer(){};

bool Lexer::IsAtEnd() const {
    return m_Current >= Source.length();
}

bool Lexer::Match(char expected){};

bool Lexer::IsDigit() const {};
bool Lexer::IsAlphaNumeric() const {};

char Lexer::Advance(){
    m_Current++;
    return Source[m_Current - 1];
};
char Lexer::Peek(){
};
char Lexer::PeekNext(){};

