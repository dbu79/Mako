#pragma once

#include <vector>
#include <unordered_map>

#include "../Parser/Statements/Stmt.h"
#include "../Parser/Expressions/Expr.h"
#include "../Interpreter/Interpreter.h"
#include "../Lexer/Token.h"

class Resolver : public Expr::Visitor, public Stmt::Visitor {
    private:
        Interpreter& interpreter;
        std::vector<std::unordered_map<std::string, bool>> scopes;

        enum FunctionType {
            NONE, FUNCTION, INITIALIZER, METHOD
        };

        FunctionType currentFunction = NONE;
        
        enum ClassType {
            NONE_CLASS, CLASS 
        };

        ClassType currentClass = NONE_CLASS; 
        

        void Resolve(const Stmt& stmt);
        void Resolve(const Expr& expr);
        void Resolve(const StmtPtr& stmt);
        void Resolve(const ExprPtr& expr);
        void BeginScope();
        void EndScope();
        void Declare(const Token& name);
        void Define(const Token& name);
        void ResolveLocal(const Expr* expr, const Token& name);
        void ResolveFunction(const Stmt::Function& function, FunctionType type);
    public:
        Resolver(Interpreter& interpreter) : interpreter(interpreter) {}
        
        void VisitBlockStmt(const Stmt::Block& stmt) override;
        void VisitClassStmt(const Stmt::Class& stmt) override;
        void VisitVarStmt(const Stmt::Var& stmt) override;
        void VisitFunctionStmt(const Stmt::Function& stmt) override;
        void VisitExpressionStmt(const Stmt::Expression& stmt) override;
        void VisitIfStmt(const Stmt::If& stmt) override;
        void VisitPrintStmt(const Stmt::Print& stmt) override;
        void VisitReturnStmt(const Stmt::Return& stmt) override;
        void VisitWhileStmt(const Stmt::While& stmt) override;

        std::any VisitVariableExpr(const Expr::Variable& expr) override;
        std::any VisitAssignExpr(const Expr::Assign& expr) override;
        std::any VisitBinaryExpr(const Expr::Binary& expr) override;
        std::any VisitCallExpr(const Expr::Call& expr) override;
        std::any VisitGetExpr(const Expr::Get& expr) override;
        std::any VisitGroupingExpr(const Expr::Grouping& expr) override;
        std::any VisitLiteralExpr(const Expr::Literal& expr) override;
        std::any VisitSetExpr(const Expr::Set& expr) override;
        std::any VisitThisExpr(const Expr::This& expr) override;
        std::any VisitLogicalExpr(const Expr::Logical& expr) override;
        std::any VisitUnaryExpr(const Expr::Unary& expr) override;

        void Resolve(const std::vector<StmtPtr>& statements);
};


