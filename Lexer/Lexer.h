#include <vector>
#include <string>
#include "Token.cpp"

class Lexer {
    public: 
        Lexer(const std::string& source); 
        std::vector<Token> scanTokens();

    private:
        void scanToken();
        void addToken(TokenType type);

        void identifer(); 
        void number(); 
        void string(); 

        bool isAtEnd() const;
        bool match(char expected);
        
        bool isDigit() const;
        bool isAlphaNumeric() const; 

        char advance();
        char peek();
        char peekNext();
        
        std::string source; 
        int line = 1; 

};
