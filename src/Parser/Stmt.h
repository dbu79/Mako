#pragma once

#include <memory>
#include <utility>
#include <vector>
#include "Expr.h"
#include "../Lexer/Token.h"
#include "StmtPtr.h"

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
    class Visitor;

    virtual ~Stmt() = default;
    virtual void Accept(Visitor& visitor) const = 0;
};

class Stmt::Visitor {
public:
    virtual ~Visitor() = default;
    virtual void VisitBlockStmt(const Block& stmt) = 0;
    virtual void VisitExpressionStmt(const Expression& stmt) = 0;
    virtual void VisitFunctionStmt(const Function& stmt) = 0;
    virtual void VisitClassStmt(const Class& stmt) = 0;
    virtual void VisitIfStmt(const If& stmt) = 0;
    virtual void VisitPrintStmt(const Print& stmt) = 0;
    virtual void VisitReturnStmt(const Return& stmt) = 0;
    virtual void VisitVarStmt(const Var& stmt) = 0;
    virtual void VisitWhileStmt(const While& stmt) = 0;
};

class Stmt::Block : public Stmt {
public:
    Block(std::vector<StmtPtr> statements)
        : statements(std::move(statements)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitBlockStmt(*this);
    }

    std::vector<StmtPtr> statements;
};

class Stmt::Expression : public Stmt {
public:
    Expression(ExprPtr expression)
        : expression(std::move(expression)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitExpressionStmt(*this);
    }

    ExprPtr expression;
};

class Stmt::Function : public Stmt {
public:
    Function(Token name, std::vector<Token> params, std::vector<StmtPtr> body)
        : name(std::move(name)),
          params(std::move(params)),
          body(std::move(body)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitFunctionStmt(*this);
    }

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

    void Accept(Visitor& visitor) const override {
        visitor.VisitClassStmt(*this);
    }

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

    void Accept(Visitor& visitor) const override {
        visitor.VisitIfStmt(*this);
    }

    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;
};

class Stmt::Print : public Stmt {
public:
    Print(ExprPtr expression)
        : expression(std::move(expression)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitPrintStmt(*this);
    }

    ExprPtr expression;
};

class Stmt::Return : public Stmt {
public:
    Return(Token keyword, ExprPtr value)
        : keyword(std::move(keyword)),
          value(std::move(value)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitReturnStmt(*this);
    }

    Token keyword;
    ExprPtr value;
};

class Stmt::Var : public Stmt {
public:
    Var(Token name, ExprPtr initializer)
        : name(std::move(name)),
          initializer(std::move(initializer)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitVarStmt(*this);
    }

    Token name;
    ExprPtr initializer;
};

class Stmt::While : public Stmt {
public:
    While(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)),
          body(std::move(body)) {}

    void Accept(Visitor& visitor) const override {
        visitor.VisitWhileStmt(*this);
    }

    ExprPtr condition;
    StmtPtr body;
};
