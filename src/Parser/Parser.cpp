#include "Parser.h"
#include "../Error/ErrorReporter.h"
#include <initializer_list>
#include <memory>
#include <vector>

ExprPtr Parser::Expression(){
    return Assignment();
}

ExprPtr Parser::Assignment(){
    ExprPtr expr = Equality();
    
    if (Match({TokenType::EQUAL})){
        Token equals = Previous();
        ExprPtr value = Assignment();

        if (auto* varExpr = dynamic_cast<Expr::Variable*>(expr.get())){
            Token name = varExpr->name;
            return std::make_unique<Expr::Assign>(std::move(name), std::move(value));
        };

        ErrorReporter::Error(equals, "Invalid assignment target.");
    }

    return expr;
}

ExprPtr Parser::Equality(){
    ExprPtr expr = Comparison();

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
    if (Match({TokenType::IDENTIFIER})){
        return std::make_unique<Expr::Variable>(Previous());
    }
    if (Match({TokenType::LEFTPAREN})){
        ExprPtr expr = Expression(); 
        Consume(TokenType::RIGHTPAREN, "Expect ')' after expression.");
        return std::make_unique<Expr::Grouping>(std::move(expr));
    }

    throw ErrorReporter::Error(Peek(), "Expect expression.");
}

StmtPtr Parser::Statement(){
    if (Match({TokenType::PRINT})) return PrintStatement();
    if (Match({TokenType::LEFTBRACE})) return std::make_unique<Stmt::Block>(Block());
    return ExpressionStatement();
}

StmtPtr Parser::PrintStatement(){
    ExprPtr value = Expression();
    Consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<Stmt::Print>(std::move(value));
}

StmtPtr Parser::ExpressionStatement(){
    ExprPtr value = Expression();
    Consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<Stmt::Expression>(std::move(value));
}

std::vector<StmtPtr> Parser::Block(){
    std::vector<StmtPtr> statements; 

    while (!Check(TokenType::RIGHTBRACE) && !IsAtEnd()){
        statements.emplace_back(Declaration());
    }

    Consume(TokenType::RIGHTBRACE, "Expect '}' after block.");
    return statements;
}

StmtPtr Parser::VarDeclaration(){
    Token name = Consume(TokenType::IDENTIFIER, "Expect variable name.");

    ExprPtr initializer = nullptr;
    if (Match({TokenType::EQUAL})){
        initializer = Expression();
    }

    Consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_unique<Stmt::Var>(std::move(name), std::move(initializer));
}

StmtPtr Parser::Declaration(){
    try {
        if (Match({TokenType::VAR})) return VarDeclaration();
        return Statement();
    } catch (const ErrorReporter::ParseError& error){
        Synchronize();
        return nullptr;
    }
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

void Parser::Synchronize(){
    Advance();

    while (!IsAtEnd()){
        if (Previous().Type == TokenType::SEMICOLON) return;
    
    switch (Peek().Type){
        case TokenType::CLASS:
        case TokenType::FN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        }
        Advance();
    }
}

std::vector<StmtPtr> Parser::Parse(){
    std::vector<StmtPtr> statements; 
    while (!IsAtEnd()){
        statements.emplace_back(Declaration());
    }
 
    return statements;
}

