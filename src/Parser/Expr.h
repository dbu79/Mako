#pragma once

#include <any>
#include <utility>
#include "../Lexer/Token.h"
#include "ExprPtr.h"

class Expr {
public:
    class Binary;
    class Grouping;
    class Literal;
    class Unary;
    class Visitor;

    virtual ~Expr() = default;
    virtual std::any Accept(Visitor& visitor) const = 0;
};

class Expr::Visitor {
    public:
        virtual ~Visitor() = default;
        virtual std::any VisitBinaryExpr(const Binary& expr) = 0;
        virtual std::any VisitGroupingExpr(const Grouping& expr) = 0;
        virtual std::any VisitLiteralExpr(const Literal& expr) = 0;
        virtual std::any VisitUnaryExpr(const Unary& expr) = 0;
};

class Expr::Binary : public Expr {
public:
    Binary(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right)) {}
    
    std::any Accept(Visitor& visitor) const override {
        return visitor.VisitBinaryExpr(*this);
    }

    ExprPtr left;
    Token op;
    ExprPtr right;
};

class Expr::Grouping : public Expr {
public:
    Grouping(ExprPtr expression)
        : expression(std::move(expression)) {}
    
    std::any Accept(Visitor& visitor) const override {
        return visitor.VisitGroupingExpr(*this);
    }

    ExprPtr expression;
};

class Expr::Literal : public Expr {
public:
    Literal(std::any value)
        : value(std::move(value)) {}
    
    std::any Accept(Visitor& visitor) const override {
        return visitor.VisitLiteralExpr(*this);
    }

    std::any value;
};

class Expr::Unary : public Expr {
public:
    Unary(Token op, ExprPtr right)
        : op(std::move(op)),
          right(std::move(right)) {}

    std::any Accept(Visitor& visitor) const override {
        return visitor.VisitUnaryExpr(*this);
    }

    Token op;
    ExprPtr right;
};

