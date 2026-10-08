#pragma once
#include <any>
#include <memory>
#include <string>
#include <vector>
#include "Callable/Callable.h"

class Interpreter;  

class MakoClass : public MakoCallable, public std::enable_shared_from_this<MakoClass> {
    public:
        const std::string name;
        explicit MakoClass(std::string name);

        std::string ToString() const override;
        std::any Call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
        size_t Arity() const override;
};
