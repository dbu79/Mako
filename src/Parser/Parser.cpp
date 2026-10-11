#include "Parser.h"
#include "../Error/ErrorReporter.h"
#include "../Lexer/TokenType.h"
#include <initializer_list>
#include <memory>
#include <vector>

ExprPtr Parser::Expression(){
    return Assignment();
}

ExprPtr Parser::Assignment(){
    ExprPtr expr = Or();
    
    if (Match({TokenType::EQUAL})){
        Token equals = Previous();
        ExprPtr value = Assignment();

        if (auto* varExpr = dynamic_cast<Expr::Variable*>(expr.get())){
            Token name = varExpr->name;
            return std::make_unique<Expr::Assign>(std::move(name), std::move(value));
        } else if (auto* get = dynamic_cast<Expr::Get*>(expr.get())){
            return std::make_unique<Expr::Set>(std::move(get->object), get->name, std::move(value));
        }

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

    return Postfix();
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
    if (Match({TokenType::THIS})){
        return std::make_unique<Expr::This>(Previous());
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

ExprPtr Parser::Postfix(){
    ExprPtr expr = Call();

    if (Match({TokenType::PLUSPLUS, TokenType::MINUSMINUS})){
        Token op = Previous();

        auto* var = dynamic_cast<Expr::Variable*>(expr.get());

        if (!var){
            ErrorReporter::Error(op, "Invalid increment target.");
            return expr;
        }

        Token name = var->name;
        bool inc = op.Type == TokenType::PLUSPLUS;
        Token postfixOp(inc ? TokenType::PLUS : TokenType::MINUS, inc ? "+" : "-", nullptr, op.Line);
        
        ExprPtr one = std::make_unique<Expr::Literal>(1.0);
        ExprPtr sum = std::make_unique<Expr::Binary>(std::move(expr), postfixOp, std::move(one));
        return std::make_unique<Expr::Assign>(name, std::move(sum));
    }
    return expr;
}

ExprPtr Parser::Or(){
    ExprPtr expr = And();

    while (Match({TokenType::OR})){
        Token op = Previous();
        ExprPtr right = And();
        expr = std::make_unique<Expr::Logical>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::And(){
    ExprPtr expr = Equality();

    while (Match({TokenType::AND})){
        Token op = Previous();
        ExprPtr right = Equality();
        expr = std::make_unique<Expr::Logical>(std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

ExprPtr Parser::Call(){
    ExprPtr expr = Primary();

    while (true){
        if (Match({TokenType::LEFTPAREN})){
            expr = FinishCall(std::move(expr));
        } else if (Match({TokenType::DOT})){
            Token name = Consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
            expr = std::make_unique<Expr::Get>(std::move(expr), std::move(name));
        } else {
            break;
        }
    }

    return expr;
}

ExprPtr Parser::FinishCall(ExprPtr callee){
    std::vector<ExprPtr> arguments;
    if (!Check(TokenType::RIGHTPAREN)){
        do {
            if (arguments.size() >= 255){
                ErrorReporter::Error(Peek(), "Can't have more than 255 arguments");
            }
            arguments.push_back(Expression());
        } while (Match({TokenType::COMMA}));
    }

    Token paren = Consume(TokenType::RIGHTPAREN, "Expect ')' after arguments.");
    return std::make_unique<Expr::Call>(std::move(callee), std::move(paren), std::move(arguments));
}

StmtPtr Parser::Statement(){
    if (Match({TokenType::FOR})) return ForStatement();
    if (Match({TokenType::IF})) return IfStatement();
    if (Match({TokenType::PRINT})) return PrintStatement();
    if (Match({TokenType::RETURN})) return ReturnStatement();
    if (Match({TokenType::WHILE})) return WhileStatement();
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

StmtPtr Parser::IfStatement(){
    Consume(TokenType::LEFTPAREN, "Expect '(' after 'if'.");
    ExprPtr condition = Expression();
    Consume(TokenType::RIGHTPAREN, "Expect ')' after if condition.");

    StmtPtr thenBranch = Statement();
    StmtPtr elseBranch = nullptr;
    if (Match({TokenType::ELSE})){
        elseBranch = Statement();
    }

    return std::make_unique<Stmt::If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

StmtPtr Parser::ReturnStatement(){
    Token keyword = Previous();
    ExprPtr value = nullptr;
    if (!Check(TokenType::SEMICOLON)){
        value = Expression();
    }

    Consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<Stmt::Return>(std::move(keyword), std::move(value));
}

StmtPtr Parser::WhileStatement(){
    Consume(TokenType::LEFTPAREN, "Expect '(' after 'while'.");
    ExprPtr condition = Expression();
    Consume(TokenType::RIGHTPAREN, "Expect ')' after condition.");
    StmtPtr body = Statement();

    return std::make_unique<Stmt::While>(std::move(condition), std::move(body));
}

StmtPtr Parser::ForStatement(){
    Consume(TokenType::LEFTPAREN, "Expect '(' after 'for'.");

    StmtPtr initializer;
    if (Match({TokenType::SEMICOLON})){
        initializer = nullptr;
    } else if (Match({TokenType::VAR})){
        initializer = VarDeclaration();
    } else {
        initializer = ExpressionStatement();
    }
    
    ExprPtr condition = nullptr;
    if (!Check(TokenType::SEMICOLON)){
        condition = Expression();
    }
    Consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");
    
    ExprPtr increment = nullptr;
    if (!Check(TokenType::RIGHTPAREN)){
        increment = Expression();
    }
    Consume(TokenType::RIGHTPAREN, "Expect ')' after for clauses.");

    StmtPtr body = Statement();
    
    if (increment != nullptr){
        std::vector<StmtPtr> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<Stmt::Expression>(std::move(increment)));

        body = std::make_unique<Stmt::Block>(std::move(statements));
    }

    if (condition == nullptr) condition = std::make_unique<Expr::Literal>(true);
    body = std::make_unique<Stmt::While>(std::move(condition), std::move(body));

    if (initializer != nullptr){
        std::vector<StmtPtr> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Stmt::Block>(std::move(statements));
    }

    return body;
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

std::unique_ptr<Stmt::Function> Parser::Function(const std::string &kind){
    Token name = Consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

    Consume(TokenType::LEFTPAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> params;
    if (!Check(TokenType::RIGHTPAREN)){
        do {
            if (params.size() >= 255){
                ErrorReporter::Error(Peek(), "Can't have more than 255 parameters.");
            }
            params.push_back(Consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (Match({TokenType::COMMA}));
    }
    Consume(TokenType::RIGHTPAREN, "Expect ')' after parameters.");
    Consume(TokenType::LEFTBRACE, "Expect '{' before " + kind + " body.");
    std::vector<StmtPtr> body = Block();
    return std::make_unique<Stmt::Function>(std::move(name), std::move(params), std::move(body));
}

StmtPtr Parser::Declaration(){
    try {
        if (Match({TokenType::CLASS})) return ClassDeclaration();
        if (Match({TokenType::FN})) return Function("function");
        if (Match({TokenType::VAR})) return VarDeclaration();
        return Statement();
    } catch (const ErrorReporter::ParseError& error){
        Synchronize();
        return nullptr;
    }
}

StmtPtr Parser::ClassDeclaration(){
    Token name = Consume(TokenType::IDENTIFIER, "Expect class name.");
    Consume(TokenType::LEFTBRACE, "Expect '{' before class body.");

    std::vector<std::unique_ptr<Stmt::Function>> methods; 
    while (!Check(TokenType::RIGHTBRACE) && !IsAtEnd()){
        Consume(TokenType::FN, "Expect 'fn' before method name");
        methods.push_back(Function("method"));
    }

    Consume(TokenType::RIGHTBRACE, "Expect '}' after class body.");
    return std::make_unique<Stmt::Class>(std::move(name), std::move(methods));
}

std::vector<StmtPtr> Parser::Block(){
    std::vector<StmtPtr> statements; 

    while (!Check(TokenType::RIGHTBRACE) && !IsAtEnd()){
        statements.emplace_back(Declaration());
    }

    Consume(TokenType::RIGHTBRACE, "Expect '}' after block.");
    return statements;
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

