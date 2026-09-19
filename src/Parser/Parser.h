#include "../Lexer/Token.h"
#include "Expr.h"
#include "../Lexer/TokenType.h"
#include "ExprPtr.h"
#include <vector>

class Parser {
    private:
        const std::vector<Token> m_Tokens; 
        size_t m_Current = 0;
        
        ExprPtr Expression();
        ExprPtr Equality();       
        ExprPtr Comparison();

        bool Match(std::initializer_list<TokenType> types);
        bool Check(TokenType type);
        bool IsAtEnd();

        Token Peek();
        Token Previous();
        Token Advance();

    public:
        Parser(const std::vector<Token>& tokens) : m_Tokens(tokens) {};

};

