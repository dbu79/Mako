#include "Parser.h"
#include "../Error/ErrorReporter.h"
#include <initializer_list>
#include <memory>

ExprPtr Parser::Expression(){
    return Parser::Equality();
}

ExprPtr Parser::Equality(){
    std::unique_ptr<Expr> expr = Comparison();

    while (Match({TokenType::BANGEQUAL, TokenType::EQUALEQUAL})){
        Token op = Previous();
        ExprPtr right = Comparison();
        expr = std::make_unique<Expr::Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprPtr Parser::Comparison(){
    ExprPtr expr = Term();

    while (Match({TokenType::GREATER, TokenType::GREATEREQUAL, TokenType::LESS, TokenType::LESSEQUAL})){
        Token op = Previous();
        ExprPtr right = Term();
        expr = std::make_unique<Expr::Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprPtr Parser::Term(){
    ExprPtr expr = Factor();
    
    while(Match({TokenType::MINUS, TokenType::PLUS})){
        Token op = Previous();
        ExprPtr right = Factor();
        expr = std::make_unique<Expr::Binary>(std::move(expr), op, std::move(right));
    }

    return expr;
}

ExprPtr Parser::Factor(){
    ExprPtr expr = Unary();

    while (Match({TokenType::SLASH, TokenType::STAR})){
        Token op = Previous();
        ExprPtr right = Unary();
        expr = std::make_unique<Expr::Binary>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::Unary(){
    if (Match({TokenType::BANG, TokenType::MINUS})){
        Token op = Previous();
        ExprPtr right = Unary();
        return std::make_unique<Expr::Unary>(op, std::move(right));
    }

    return Primary();
}

ExprPtr Parser::Primary(){
    if (Match({TokenType::FALSE})){
        return std::make_unique<Expr::Literal>(false);
    }
    if (Match({TokenType::TRUE})){
        return std::make_unique<Expr::Literal>(true);
    }
    if (Match({TokenType::NIL})){
        return std::make_unique<Expr::Literal>(nullptr);
    }

    if (Match({TokenType::NUMBER, TokenType::STRING})){
        return std::make_unique<Expr::Literal>(Previous().Literal);
    }

    if (Match({TokenType::LEFTPAREN})){
        ExprPtr expr = Expression(); // Add Expression
        Consume(TokenType::RIGHTPAREN, "Expect ')' after expression.");
        return std::make_unique<Expr::Grouping>(std::move(expr));
    }

    throw ErrorReporter::Error(Peek(), "Expect expression.");
}

bool Parser::Match(std::initializer_list<TokenType> types){
    for (TokenType type : types) {
        if (Check(type)){
            Advance();
            return true;
        }
    }

    return false;
}

bool Parser::Check(TokenType type) const {
    if (IsAtEnd()) return false;
    return Peek().Type == type;
}


bool Parser::IsAtEnd() const {
    return Peek().Type == TokenType::END_OF_FILE;
}

const Token& Parser::Peek() const {
    return m_Tokens.at(m_Current);
}

const Token& Parser::Previous() const {
    return m_Tokens.at(m_Current - 1);
}

Token Parser::Advance(){
    if (!IsAtEnd()) m_Current++;
    return Previous();
}

Token Parser::Consume(TokenType type, std::string message){
    if (Check(type)) return Advance();
    
    throw ErrorReporter::Error(Peek(), message);
}
