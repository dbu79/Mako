#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <memory>
#include "../Lexer/Token.h"

class Environment {
    private:
        std::unordered_map<std::string, std::any> values; 
        std::shared_ptr<Environment> enclosing;

    public:
        void Define(const std::string& name, const std::any& value);
        void Assign(const Token& name, const std::any& value);
        std::any Get(const Token& name);
        std::any getAt(int distance, const std::string& name);
        void assignAt(int distance, const Token& name, const std::any& value);
        
        Environment* Ancestor(int distance);
        Environment(std::shared_ptr<Environment> enclosing = nullptr) 
            : enclosing(std::move(enclosing)){} 
};

