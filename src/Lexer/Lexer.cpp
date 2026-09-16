#include <vector>
#include <any>
#include "Lexer.h"
#include "TokenType.h"
#include "../Error/ErrorReporter.h"
#include "Keywords.h"

std::vector<Token> Lexer::ScanTokens(){
    while (!IsAtEnd()){
        m_Start = m_Current; 
        ScanToken();
    }
    Tokens_.emplace_back(TokenType::END_OF_FILE, "", nullptr, m_Line);
    return Tokens_;
};

void Lexer::ScanToken(){
    char c {Advance()};
    switch (c){
        // Single Char Tokens
        case '(': AddToken(TokenType::LEFTPAREN); break; 
        case ')': AddToken(TokenType::RIGHTPAREN); break; 
        case '{': AddToken(TokenType::LEFTBRACE); break; 
        case '}': AddToken(TokenType::RIGHTBRACE); break; 
        case '*': AddToken(TokenType::STAR); break; 
        case ',': AddToken(TokenType::COMMA); break; 
        case '.': AddToken(TokenType::DOT); break; 
        case ':': AddToken(TokenType::COLON); break; 
        case ';': AddToken(TokenType::SEMICOLON); break; 
        case '/': 
        // Handle Comments
            if (Match('/')){
                while(Peek() != '\n' && !IsAtEnd()) Advance();
            } else {
                AddToken(TokenType::SLASH);
            } break;
        
        // Multi Char Tokens
        case '!': 
            AddToken(Match('=') ? TokenType::BANGEQUAL : TokenType::BANG); 
            break; 
        case '>': 
            AddToken(Match('=') ? TokenType::GREATEREQUAL : TokenType::GREATER);
            break;
        case '<':
            AddToken(Match('=') ? TokenType::LESSEQUAL : TokenType::LESS);
            break;
        case '+': 
            AddToken(Match('+') ? TokenType::PLUSPLUS : (Match('=') ? TokenType::PLUSEQUAL : TokenType::PLUS)); 
            break; 
        case '-': 
            AddToken(Match('-') ? TokenType::MINUSMINUS : (Match('=') ? TokenType::MINUSEQUAL : TokenType::MINUS)); 
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
        case '"': String(); break;

        // Default case
        default: {
            if (IsDigit(c)){
                Number();
            } else if (IsAlpha(c)){
                Identifer();
            } else {
                ErrorReporter::Error(m_Line, "Unexpected character.");
            }
        }
    }
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
    
    std::string text = Source.substr(m_Start, m_Current - m_Start);
    if (Keywords.find(text) != Keywords.end()){
        AddToken(Keywords.at(text));
        return;
    }

    AddToken(TokenType::IDENTIFIER);
};

void Lexer::Number(){
    while (IsDigit(Peek())) Advance();

    if (Peek() == '.' && IsDigit(PeekNext())){
        Advance();
        while (IsDigit(Peek())) Advance();
    };

    AddToken(TokenType::NUMBER);
};

void Lexer::String(){
    while (Peek() != '"' && !IsAtEnd()){
        if (Peek() == '\n') {
            m_Line++;
        };
        Advance();
    }

    if (IsAtEnd()){
        ErrorReporter::Error(m_Line, "Unterminated string.");
        return;
    }

    Advance();

    std::string value = Source.substr(m_Start + 1, (m_Current - m_Start) - 2);
    AddToken(TokenType::STRING, value);

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

