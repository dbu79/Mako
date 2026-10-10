#pragma once

#include "Interpreter/Interpreter.h"
#include "Parser/Statements/Stmt.h"
#include "Environment/Environment.h"
#include <chrono>

class Interpreter;

class MakoCallable {
    public:
        virtual ~MakoCallable() = default;
        virtual size_t Arity() const = 0;
        virtual std::any Call(Interpreter& interpreter, const std::vector<std::any>& arguments) = 0;
        virtual std::string ToString() const = 0;

};

class NativeClock : public MakoCallable {
    public:
        size_t Arity() const override { return 0; }

        std::any Call(Interpreter&, 
                const std::vector<std::any>&) override {
            using namespace std::chrono;
            double seconds = duration<double>(system_clock::now().time_since_epoch()).count();
            return seconds;
        };

    std::string ToString() const override { return "<native fn>"; };
};

class MakoFunction : public MakoCallable {
    public:
        explicit MakoFunction(const Stmt::Function *declaration, std::shared_ptr<Environment> closure)
            : declaration(declaration), closure(std::move(closure)) {}

        size_t Arity() const override { return declaration->params.size(); }

        std::any Call(Interpreter& interpreter, 
                const std::vector<std::any>& arguments) override;

        std::string ToString() const override {
            return "<fn " + declaration->name.Lexeme + ">";
        }
    private:
        const Stmt::Function *declaration;
        const Environment closure;
    
};
