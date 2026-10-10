#include "MakoFunction.h"
#include "Environment/Environment.h"
#include "Return.h"
#include <memory>

std::shared_ptr<MakoFunction> MakoFunction::Bind(std::shared_ptr<MakoInstance> instance) const {
    auto environment = std::make_shared<Environment>(closure);
    environment->Define("this", instance);
    return std::make_shared<MakoFunction>(declaration, environment);
}

std::any MakoFunction::Call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto environment = std::make_shared<Environment>(closure);

    for (size_t i = 0; i < declaration->params.size(); i++){
        environment->Define(declaration->params[i].Lexeme, arguments[i]);
    }
    
    try {
        interpreter.ExecuteBlock(declaration->body, environment);
    } catch (const Return &returnValue){
        return returnValue.value;
    }

    return std::any{};
}

