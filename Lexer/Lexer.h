#pragma once

#include <vector>
#include <string>
#include "Token.h"

class Lexer {
    public: 
        Lexer(const std::string& source) : Source(source){}; 
        std::vector<Token> ScanTokens();

    private:
        void ScanToken();
        void AddToken(TokenType type);

        void Identifer(); 
        void Number(); 
        void String(); 

        bool IsAtEnd() const;
        bool Match(char expected);
        
        bool IsDigit() const;
        bool IsAlphaNumeric() const; 

        char Advance();
        char Peek();
        char PeekNext();
        
        const std::string Source; 
        std::vector<Token> Tokens_ {};

        size_t m_Start{}; 
        size_t m_Current{};
        size_t m_Line{1}; 
};
