#pragma once

#include "../Lexer/Token.h"
#include "Expr.h"
#include "../Lexer/TokenType.h"
#include "Stmt.h"
#include <vector>

class Parser {
    private:
        const std::vector<Token> m_Tokens; 
        size_t m_Current = 0;
        
        ExprPtr Assignment();
        ExprPtr Expression();
        ExprPtr Equality();       
        ExprPtr Comparison();
        ExprPtr Term();
        ExprPtr Factor();
        ExprPtr Unary();
        ExprPtr Primary();
        ExprPtr Or();
        ExprPtr And();

        StmtPtr Statement();
        StmtPtr PrintStatement();
        StmtPtr ExpressionStatement();
        StmtPtr IfStatement();
        std::vector<StmtPtr> Block();
        StmtPtr VarDeclaration();
        StmtPtr Declaration();

        bool Match(std::initializer_list<TokenType> types);
        bool Check(TokenType type) const;
        bool IsAtEnd() const;

        const Token& Peek() const;
        const Token& Previous() const;
        Token Advance();
        Token Consume(TokenType type, std::string message);
        
        void Synchronize();
        
    public:
        Parser(const std::vector<Token>& tokens) : m_Tokens(tokens) {};
        std::vector<StmtPtr> Parse();      
};

