#include "Interpreter.h"
#include "../Error/ErrorReporter.h"
#include <any>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

void Interpreter::Interpret(const Expr& expr){
    try {
        std::any value = Evaluate(expr);
        std::cout << Stringify(value) << "\n";
    } catch (const ErrorReporter::RuntimeError& error){
        ErrorReporter::ReportRuntimeError(error);
    }
}

std::string Interpreter::Stringify(const std::any &object){
    if (!object.has_value() || object.type() == typeid(std::nullptr_t)) return "nil";

    if (object.type() == typeid(double)) {
        double double_ = std::any_cast<double>(object);
        std::ostringstream ss;
        ss << double_;
        std::string text = ss.str();

        if (text.size() >= 2 && text.compare(text.size() - 2, 2, ".0") == 0) {
            text = text.substr(0, text.size() - 2);
        }
        return text;
    }

    if (object.type() == typeid(bool)){
        return std::any_cast<bool>(object) ? "true" : "false";
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    return "<?>";
}

std::any Interpreter::VisitLiteralExpr(const Expr::Literal& expr){
    return expr.value;
}

std::any Interpreter::VisitGroupingExpr(const Expr::Grouping& expr){
    return Evaluate(*expr.expression);
}

std::any Interpreter::VisitUnaryExpr(const Expr::Unary& expr){
    std::any right = Evaluate(*expr.right);

    switch (expr.op.Type) {
        case TokenType::BANG:
            return !IsTruthy(right);
        case TokenType::MINUS:
            CheckNumberOperand(expr.op, right);
            return -std::any_cast<double>(right);
        default: break;
    };

    return nullptr;
}

std::any Interpreter::VisitBinaryExpr(const Expr::Binary& expr){
    std::any left = Evaluate(*expr.left);
    std::any right = Evaluate(*expr.right);

    switch (expr.op.Type){
        case TokenType::MINUS:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::PLUS:
            CheckNumberOperands(expr.op, left, right);
            if (left.type() == typeid(double) && right.type() == typeid(double)){
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string) && right.type() == typeid(std::string)){
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
        case TokenType::SLASH:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::STAR:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::GREATER:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATEREQUAL:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESSEQUAL:
            CheckNumberOperands(expr.op, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case TokenType::BANGEQUAL: 
            return !IsEqual(left, right);
        case TokenType::EQUALEQUAL:
            return IsEqual(left, right);
        default: break;
        }

    return nullptr;
}

bool Interpreter::IsTruthy(const std::any& object){
    if (!object.has_value() || object.type() == typeid(std::nullptr_t)) return false;
    if (object.type() == typeid(bool)) return std::any_cast<bool>(object);
    return true;
}

bool Interpreter::IsEqual(const std::any& a, const std::any& b){
    if (a.type() == typeid(std::nullptr_t) && b.type() == typeid(std::nullptr_t)){
        return true;
    }
    if (a.type() == typeid(std::nullptr_t)){
        return false;
    }

    return a.type() == b.type();
}

void Interpreter::CheckNumberOperand(const Token &op, const std::any &operand){
    if (operand.type() == typeid(double)) return;
    throw ErrorReporter::RuntimeError(op, "Operand must be a number.");
}

void Interpreter::CheckNumberOperands(const Token &op, const std::any &left, const std::any &right){
    if (left.type() == typeid(double) && right.type() == typeid(double)) return;
    throw ErrorReporter::RuntimeError(op, "Operands must be a number.");
}

