#pragma once

#include "../Parser/Expr.h"
#include "../Parser/Stmt.h"
#include "../Environment/Environment.h"
#include <string>

class Interpreter : public Expr::Visitor, public Stmt::Visitor {
    public:
        std::any VisitBinaryExpr(const Expr::Binary& expr) override;
        std::any VisitGroupingExpr(const Expr::Grouping& expr) override;
        std::any VisitLiteralExpr(const Expr::Literal& expr) override;
        std::any VisitLogicalExpr(const Expr::Logical& expr) override;
        std::any VisitUnaryExpr(const Expr::Unary& expr) override;
        std::any VisitVariableExpr(const Expr::Variable& expr) override;
        std::any VisitAssignExpr(const Expr::Assign& expr) override;

        void VisitExpressionStmt(const Stmt::Expression& stmt) override;
        void VisitPrintStmt(const Stmt::Print& stmt) override;
        void VisitVarStmt(const Stmt::Var& stmt) override;
        void VisitBlockStmt(const Stmt::Block& stmt) override;
        // void VisitFunctionStmt(const Stmt::Function& stmt) override;
        // void VisitClassStmt(const Stmt::Class& stmt) override;
        void VisitIfStmt(const Stmt::If& stmt) override;
        // void VisitReturnStmt(const Stmt::Return& stmt) override;
        void VisitWhileStmt(const Stmt::While& stmt) override;

        void Interpret(const std::vector<StmtPtr>& expr);
    private:
        Environment environment;

        std::any Evaluate(const Expr& expr) { return expr.Accept(*this);}
        void Execute(const StmtPtr& stmt) { return stmt->Accept(*this);}
        void ExecuteBlock(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> environment);

        bool IsTruthy(const std::any& object);
        bool IsEqual(const std::any& a, const std::any&  b);

        void CheckNumberOperand(const Token& op, const std::any& operand);
        void CheckNumberOperands(const Token& op, const std::any& operand1, const std::any& operand2);
        
        std::string Stringify(const std::any& object);
}; 
