#pragma once

#include <vector>
#include <string>
#include <any>
#include "Token.h"
#include "TokenType.h"

class Lexer {
    public: 
        Lexer(const std::string& source) : Source(source){}; 
        std::vector<Token> ScanTokens();

    private:
        void ScanToken();
        void AddToken(TokenType type);
        void AddToken(TokenType type, std::any literal);

        void Identifer(); 
        void Number(); 
        void String(); 

        bool IsAtEnd() const;
        bool Match(char expected);
        
        bool IsDigit(char c) const;
        bool IsAlpha(char c) const; 
        bool IsAlphaNumeric(char c) const;


        char Advance();
        char Peek();
        char PeekNext();
        
        const std::string Source; 
        std::vector<Token> Tokens_ {};

        size_t m_Start{}; 
        size_t m_Current{};
        size_t m_Line{1}; 
};
