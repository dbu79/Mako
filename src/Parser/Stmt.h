#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>
#include "Expr.h"
#include "../Lexer/Token.h"

class Stmt {
public:
    class Block;
    class Expression;
    class Function;
    class Class;
    class If;
    class Print;
    class Return;
    class Var;
    class While;

    virtual ~Stmt() = default;
};

using StmtPtr = std::unique_ptr<Stmt>;

class Stmt::Block : public Stmt {
public:
    Block(std::vector<StmtPtr> statements)
        : statements(std::move(statements)) {}
  
    std::vector<StmtPtr> statements;
};

class Stmt::Expression : public Stmt {
public:
    Expression(ExprPtr expression)
        : expression(std::move(expression)) {}

    ExprPtr expression;
};

class Stmt::Function : public Stmt {
public:
    Function(Token name, std::vector<Token> params, std::vector<StmtPtr> body)
        : name(std::move(name)),
          params(std::move(params)),
          body(std::move(body)) {}

    Token name;
    std::vector<Token> params;
    std::vector<StmtPtr> body;
};

class Stmt::Class : public Stmt {
public:
    Class(Token name, std::unique_ptr<Expr::Variable> superclass, std::vector<std::unique_ptr<Stmt::Function>> methods)
        : name(std::move(name)),
          superclass(std::move(superclass)),
          methods(std::move(methods)) {}

    Token name;
    std::unique_ptr<Expr::Variable> superclass;
    std::vector<std::unique_ptr<Stmt::Function>> methods;
};

class Stmt::If : public Stmt {
public:
    If(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}

    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;
};

class Stmt::Print : public Stmt {
public:
    Print(ExprPtr expression)
        : expression(std::move(expression)) {}

    ExprPtr expression;
};

class Stmt::Return : public Stmt {
public:
    Return(Token keyword, ExprPtr value)
        : keyword(std::move(keyword)),
          value(std::move(value)) {}

    Token keyword;
    ExprPtr value;
};

class Stmt::Var : public Stmt {
public:
    Var(Token name, ExprPtr initializer)
        : name(std::move(name)),
          initializer(std::move(initializer)) {}

    Token name;
    ExprPtr initializer;
};

class Stmt::While : public Stmt {
public:
    While(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)),
          body(std::move(body)) {}

    ExprPtr condition;
    StmtPtr body;
};

