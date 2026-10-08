#pragma once

#include <any>
#include <string>
#include <memory>
#include <unordered_map>
#include "Lexer/Token.h"

class MakoClass;

class MakoInstance {
    public: 
        explicit MakoInstance(std::shared_ptr<MakoClass> klass);

        std::string ToString() const;
        
        std::any Get(const Token& name);
    private: 
        std::shared_ptr<MakoClass> klass;
        std::unordered_map<std::string, std::any> fields;      
};
