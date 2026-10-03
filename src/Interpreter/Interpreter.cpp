#include "Interpreter.h"
#include "../Error/ErrorReporter.h"
#include "../Callable/Callable.h"
#include "src/Return.h"
#include <any>
#include <cstddef>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

Interpreter::Interpreter()
    : globals(std::make_shared<Environment>()), environment(globals) {
        globals->Define("clock", std::shared_ptr<MakoCallable>(std::make_shared<NativeClock>()));
    }

void Interpreter::Interpret(const std::vector<StmtPtr>& statements){
    try {
        for (const StmtPtr& statement : statements){
            Execute(statement);
        }
    } catch (const ErrorReporter::RuntimeError& error){
        ErrorReporter::ReportRuntimeError(error);
    }
}

std::any Interpreter::VisitGroupingExpr(const Expr::Grouping& expr){
    return Evaluate(*expr.expression);
}

std::any Interpreter::VisitLiteralExpr(const Expr::Literal& expr){
    return expr.value;
}

std::any Interpreter::VisitLogicalExpr(const Expr::Logical& expr){
    std::any left = Evaluate(*expr.left);

    if (expr.op.Type == TokenType::OR){
        if (IsTruthy(left)) return left;
    } else {
        if (!IsTruthy(left)) return left;
    }

    return Evaluate(*expr.right);
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
            if (left.type() == typeid(double) && right.type() == typeid(double)){
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }
            if (left.type() == typeid(std::string) && right.type() == typeid(std::string)){
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }
            throw ErrorReporter::RuntimeError(expr.op, "Operands must be two numbers or two strings.");
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

std::any Interpreter::VisitCallExpr(const Expr::Call &expr){
    std::any callee = Evaluate(*expr.callee);

    std::vector<std::any> arguments;
    for (const ExprPtr& argument : expr.arguments){
        arguments.push_back(Evaluate(*argument));
    }
    
    if (callee.type() != typeid(std::shared_ptr<MakoCallable>)){
        throw ErrorReporter::RuntimeError(expr.paren, "Can only call functions and classes");
    }
    auto function = std::any_cast<std::shared_ptr<MakoCallable>>(callee);

    if (arguments.size() != function->Arity()){
        throw ErrorReporter::RuntimeError(expr.paren, 
                "Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    }
    
    return function->Call(*this, arguments);
}

std::any Interpreter::VisitVariableExpr(const Expr::Variable &expr){
    return environment->Get(expr.name);
}

std::any Interpreter::VisitAssignExpr(const Expr::Assign &expr){
    std::any value = Evaluate(*expr.value);
    environment->Assign(expr.name, value);
    return value;
}
void Interpreter::VisitExpressionStmt(const Stmt::Expression &stmt){
    Evaluate(*stmt.expression);
}

void Interpreter::VisitFunctionStmt(const Stmt::Function &stmt){
    std::shared_ptr<MakoCallable> function = std::make_shared<MakoFunction>(&stmt, environment);
    environment->Define(stmt.name.Lexeme, function);
    return;
}

void Interpreter::VisitPrintStmt(const Stmt::Print &stmt){
    std::any value = Evaluate(*stmt.expression);
    std::cout << Stringify(value) << "\n";
}

void Interpreter::VisitVarStmt(const Stmt::Var &stmt){
    std::any value = nullptr;
    if (stmt.initializer != nullptr){
        value = Evaluate(*stmt.initializer);
    }

    environment->Define(stmt.name.Lexeme, value);
}

void Interpreter::VisitBlockStmt(const Stmt::Block &stmt){
    ExecuteBlock(stmt.statements, std::make_shared<Environment>(environment));
    return;
}

void Interpreter::VisitIfStmt(const Stmt::If &stmt){
    if (IsTruthy(Evaluate(*stmt.condition))){
        Execute(stmt.thenBranch);
    } else if (stmt.elseBranch != nullptr){
        Execute(stmt.elseBranch);
    } else {
        return;
    }
}

void Interpreter::VisitReturnStmt(const Stmt::Return &stmt){
    std::any value = nullptr;
    if (stmt.value != nullptr) value = Evaluate(*stmt.value);
    
    throw Return{std::move(value)};
}

void Interpreter::VisitWhileStmt(const Stmt::While &stmt){
    while (IsTruthy(Evaluate(*stmt.condition))){
        Execute(stmt.body);
    }
    return;
}

void Interpreter::ExecuteBlock(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> environment){
    auto previous = this->environment;
    try {
        this->environment = environment;
        
        for (const StmtPtr& statement : statements){
            Execute(statement);
        }
    } catch (...) {
        this->environment = previous;
        throw;
    }

    this->environment = previous;
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
