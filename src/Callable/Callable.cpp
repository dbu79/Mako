#include "Callable.h"
#include "../Environment/Environment.h"

std::any MakoFunction::Call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto environment = std::make_shared<Environment>(interpreter.globals);

    for (size_t i = 0; i < declaration->params.size(); i++){
        environment->Define(declaration->params[i].Lexeme, arguments[i]);
    }
    interpreter.ExecuteBlock(declaration->body, environment);
    return std::any{};
}
