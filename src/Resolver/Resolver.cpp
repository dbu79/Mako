#include "Resolver.h"
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
    scopes.push(std::unordered_map<std::string, bool>());
}

void Resolver::EndScope(){
   scopes.pop();
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

void Resolver::Declare(const Token& name){
    if (scopes.empty()) return;
    std::unordered_map<std::string, bool> scope = scopes.top();
    scope.insert_or_assign(name.Lexeme, false);
}

void Resolver::Define(const Token &name){
    if (scopes.empty()) return;
    scopes.top().insert_or_assign(name.Lexeme, true);
}
