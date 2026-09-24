#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include "../Lexer/Token.h"

class Environment {
    private:
        std::unordered_map<std::string, std::any> values; 
    public:
        void Define(const std::string& name, const std::any& value);
        std::any Get(const Token& name);
};
