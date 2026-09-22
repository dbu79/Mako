#pragma once

#include "../Parser/Expr.h"
#include <string>

class Interpreter : public Expr::Visitor {
    public:
        std::any VisitLiteralExpr(const Expr::Literal& expr) override;
        std::any VisitGroupingExpr(const Expr::Grouping& expr) override;
        std::any VisitUnaryExpr(const Expr::Unary& expr) override;
        std::any VisitBinaryExpr(const Expr::Binary& expr) override;

        void Interpret(const Expr& expr);
    private:
        std::any Evaluate(const Expr& expr) { return expr.Accept(*this);};
        bool IsTruthy(const std::any& object);
        bool IsEqual(const std::any& a, const std::any&  b);

        void CheckNumberOperand(const Token& op, const std::any& operand);
        void CheckNumberOperands(const Token& op, const std::any& operand1, const std::any& operand2);
        
        std::string Stringify(const std::any& object);
}; 
