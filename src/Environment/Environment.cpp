#include "Environment.h"
#include "../Error/ErrorReporter.h"

void Environment::Define(const std::string& name, const std::any& value){
    values.insert_or_assign(name, value);
}

std::any Environment::Get(const Token& name){
    if (values.contains(name.Lexeme)){
        return values.at(name.Lexeme);
    }

    throw ErrorReporter::RuntimeError(name, "Undefined variable '" + name.Lexeme + "'.");
}
