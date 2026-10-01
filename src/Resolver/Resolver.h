#pragma once

#include <vector>
#include <stack>
#include <unordered_map>

#include "../Parser/Statements/Stmt.h"
#include "../Parser/Expressions/Expr.h"
#include "../Interpreter/Interpreter.h"
#include "../Lexer/Token.h"

class Resolver : public Expr::Visitor, public Stmt::Visitor {
    private:
        Interpreter interpreter;
        std::stack<std::unordered_map<std::string, bool>> scopes;

        void BeginScope();
        void EndScope();
        void Declare(const Token& name);
        void Define(const Token& name);
        void Resolve(const StmtPtr& stmt);
        void Resolve(const ExprPtr& expr);
    public:
        Resolver(const Interpreter& interpreter) : interpreter(interpreter) {}
        
        void VisitBlockStmt(const Stmt::Block& stmt) override;
        void VisitVarStmt(const Stmt::Var& stmt) override;
        // To Do 
        void Resolve(const std::vector<StmtPtr>& statements);
};


