#pragma once

#include "../Interpreter/Interpreter.h"

class Interpreter;

class MakoCallable {
    public:
        virtual ~MakoCallable() = default;
        virtual size_t Arity() const = 0;
        virtual std::any Call(Interpreter& interpreter, const std::vector<std::any>& arguments) = 0;
};
