#include "Resolver.h"
#include "../Error/ErrorReporter.h"
#include "../Interpreter/Interpreter.h"
#include <any>
#include <unordered_map>
#include <vector>

void Resolver::Resolve(const std::vector<StmtPtr>& statements){
    for (const StmtPtr& statement : statements){
        Resolve(statement);
    }
    return;
}

void Resolver::Resolve(const StmtPtr& stmt){
    stmt->Accept(*this);
    return;
}

void Resolver::Resolve(const ExprPtr& expr){
    expr->Accept(*this);
    return;
}

void Resolver::BeginScope(){
    scopes.push_back(std::unordered_map<std::string, bool>());
}

void Resolver::EndScope(){
   scopes.pop_back();
}

void Resolver::Declare(const Token& name){
    if (scopes.empty()) return;
    auto& scope = scopes.back();
    scope.insert_or_assign(name.Lexeme, false);
}

void Resolver::Define(const Token &name){
    if (scopes.empty()) return;
    scopes.back().insert_or_assign(name.Lexeme, true);
}

void Resolver::ResolveLocal(const Expr* expr, const Token& name){
    for (int i = static_cast<int>(scopes.size()) - 1; i >= 0; i--){
        if (scopes[i].contains(name.Lexeme)){
            interpreter.Resolve(expr, static_cast<int>(scopes.size()) - 1 - i);
            return;
        }
    }
}

void Resolver::ResolveFunction(const Stmt::Function& function){
    BeginScope();
    for (const Token& param : function.params){
        Declare(param);
        Define(param);
    }
    Resolve(function.body);
    EndScope();
}

void Resolver::VisitBlockStmt(const Stmt::Block &stmt){
    BeginScope();
    Resolve(stmt.statements);
    EndScope();
    return;
}

void Resolver::VisitVarStmt(const Stmt::Var &stmt) {
    Declare(stmt.name);
    if (stmt.initializer != nullptr){
        Resolve(stmt.initializer);
    }
    Define(stmt.name);
    return;
}

void Resolver::VisitFunctionStmt(const Stmt::Function &stmt){
    Declare(stmt.name);
    Define(stmt.name);

    ResolveFunction(stmt);
}

void Resolver::VisitExpressionStmt(const Stmt::Expression &stmt){
    Resolve(stmt.expression);
    return;
}

void Resolver::VisitIfStmt(const Stmt::If &stmt){
    Resolve(stmt.condition);
    Resolve(stmt.thenBranch);
    if (stmt.elseBranch != nullptr) Resolve(stmt.elseBranch);
    return;
}

void Resolver::VisitPrintStmt(const Stmt::Print &stmt){
    Resolve(stmt.expression);
    return;
}

void Resolver::VisitReturnStmt(const Stmt::Return &stmt){
    if (stmt.value != nullptr){
        Resolve(stmt.value);
    }

    return;
}

void Resolver::VisitWhileStmt(const Stmt::While &stmt){
    Resolve(stmt.condition);
    Resolve(stmt.body);
    return;
}

std::any Resolver::VisitVariableExpr(const Expr::Variable &expr){
    if (!scopes.empty()){
        auto& scope = scopes.back();
        auto it = scope.find(expr.name.Lexeme);
        if (it != scope.end() && it->second == false){
            ErrorReporter::Error(expr.name, "Can't read local variable in its own initializer.");
        }
    }
    ResolveLocal(&expr, expr.name);
    return nullptr;

}

std::any Resolver::VisitAssignExpr(const Expr::Assign &expr){
    Resolve(expr.value);
    ResolveLocal(&expr, expr.name);
    return nullptr;
}

std::any Resolver::VisitBinaryExpr(const Expr::Binary &expr){
    Resolve(expr.left);
    Resolve(expr.right);
    return nullptr;
}

std::any Resolver::VisitCallExpr(const Expr::Call &expr){
    Resolve(expr.callee);
    
    for (const auto& argument : expr.arguments){
        Resolve(argument);
    }

    return nullptr;
}

std::any Resolver::VisitGroupingExpr(const Expr::Grouping &expr){
    Resolve(expr.expression);
    return nullptr;
}

std::any Resolver::VisitLiteralExpr(const Expr::Literal &expr){
    return nullptr;
}

std::any Resolver::VisitLogicalExpr(const Expr::Logical &expr){
    Resolve(expr.left);
    Resolve(expr.right);
    return nullptr;
}

std::any Resolver::VisitUnaryExpr(const Expr::Unary &expr){
    Resolve(expr.right);
    return nullptr;
}


