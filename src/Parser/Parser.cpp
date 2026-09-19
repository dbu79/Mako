#include "Parser.h"
#include "../Lexer/TokenType.h"
#include <initializer_list>
#include <memory>

std::unique_ptr<Expr> Parser::Expression(){
    return Parser::Equality();
};

std::unique_ptr<Expr> Parser::Equality(){
    std::unique_ptr<Expr> expr = Comparison();

    while (Match({TokenType::BANGEQUAL, TokenType::EQUALEQUAL})){
        Token op = Previous();
        std::unique_ptr<Expr> right = Comparison();
        expr = std::make_unique<Expr::Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
};

bool Parser::Match(std::initializer_list<TokenType> types){
    for (TokenType type : types) {
        if (Check(type)){
            Advance();
        }
    }

    return false;
};

bool Parser::Check(TokenType type){
    if (IsAtEnd()) return false;
    return Peek().Type == type;
}

Token Parser::Advance(){
    if (!IsAtEnd()) m_Current++;
    return Previous();
}

bool Parser::IsAtEnd(){
    return Peek().Type == TokenType::END_OF_FILE;
}

Token Parser::Peek(){
    return m_Tokens.at(m_Current);
}

