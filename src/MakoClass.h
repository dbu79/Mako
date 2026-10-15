#pragma once
#include <any>
#include <memory>
#include <string>
#include <vector>
#include "MakoFunction.h"

class Interpreter;  

class MakoClass : public MakoCallable, public std::enable_shared_from_this<MakoClass> {
    public:
        const std::string name;
        std::shared_ptr<MakoClass> superclass;
        std::unordered_map<std::string, MakoFunction> methods;

        explicit MakoClass(std::string name, std::shared_ptr<MakoClass> superclass, std::unordered_map<std::string, MakoFunction> methods);
        const MakoFunction* FindMethod(const std::string& name) const;

        std::string ToString() const override;
        std::any Call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
        size_t Arity() const override;

};
