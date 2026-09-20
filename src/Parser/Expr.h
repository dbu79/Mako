#pragma once

#include <any>
#include <memory>
#include <utility>
#include <vector>
#include "../Lexer/Token.h"

class Expr {
public:
    class Assign;
    class Binary;
    class Call;
    class Get;
    class Grouping;
    class Literal;
    class Logical;
    class Set;
    class Super;
    class This;
    class Unary;
    class Variable;

    virtual ~Expr() = default;
};

using ExprPtr = std::unique_ptr<Expr>;

class Expr::Assign : public Expr {
public:
    Assign(Token name, ExprPtr value)
        : name(std::move(name)),
          value(std::move(value)) {}

    Token name;
    ExprPtr value;
};

class Expr::Binary : public Expr {
public:
    Binary(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right)) {}

    ExprPtr left;
    Token op;
    ExprPtr right;
};

class Expr::Call : public Expr {
public:
    Call(ExprPtr callee, Token paren, std::vector<ExprPtr> arguments)
        : callee(std::move(callee)),
          paren(std::move(paren)),
          arguments(std::move(arguments)) {}

    ExprPtr callee;
    Token paren;
    std::vector<ExprPtr> arguments;
};

class Expr::Get : public Expr {
public:
    Get(ExprPtr object, Token name)
        : object(std::move(object)),
          name(std::move(name)) {}

    ExprPtr object;
    Token name;
};

class Expr::Grouping : public Expr {
public:
    Grouping(ExprPtr expression)
        : expression(std::move(expression)) {}

    ExprPtr expression;
};

class Expr::Literal : public Expr {
public:
    Literal(std::any value)
        : value(std::move(value)) {}

    std::any value;
};

class Expr::Logical : public Expr {
public:
    Logical(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right)) {}

    ExprPtr left;
    Token op;
    ExprPtr right;
};

class Expr::Set : public Expr {
public:
    Set(ExprPtr object, Token name, ExprPtr value)
        : object(std::move(object)),
          name(std::move(name)),
          value(std::move(value)) {}

    ExprPtr object;
    Token name;
    ExprPtr value;
};

class Expr::Super : public Expr {
public:
    Super(Token keyword, Token method)
        : keyword(std::move(keyword)),
          method(std::move(method)) {}

    Token keyword;
    Token method;
};

class Expr::This : public Expr {
public:
    This(Token keyword)
        : keyword(std::move(keyword)) {}

    Token keyword;
};

class Expr::Unary : public Expr {
public:
    Unary(Token op, ExprPtr right)
        : op(std::move(op)),
          right(std::move(right)) {}

    Token op;
    ExprPtr right;
};

class Expr::Variable : public Expr {
public:
    Variable(Token name)
        : name(std::move(name)) {}

    Token name;
};

