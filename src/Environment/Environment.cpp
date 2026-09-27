#include "Environment.h"
#include "../Error/ErrorReporter.h"

void Environment::Define(const std::string& name, const std::any& value){
    values.insert_or_assign(name, value);
}

void Environment::Assign(const Token& name, const std::any& value){
    if (values.contains(name.Lexeme)){
        values.insert_or_assign(name.Lexeme, value);
        return;
    }
    
    if (enclosing != nullptr){
        enclosing->Assign(name, value);
        return;
    }

    throw ErrorReporter::RuntimeError(name, "Undefined variable '" + name.Lexeme + "'.");
}

std::any Environment::Get(const Token& name){
    if (values.contains(name.Lexeme)){
        return values.at(name.Lexeme);
    }
    

    if (enclosing != nullptr) return enclosing->Get(name);

    throw ErrorReporter::RuntimeError(name, "Undefined variable '" + name.Lexeme + "'.");
}
