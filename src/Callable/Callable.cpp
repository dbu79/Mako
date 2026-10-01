#include "Callable.h"
#include "../Environment/Environment.h"
#include "../Return.h"

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
