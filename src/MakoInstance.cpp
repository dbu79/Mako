#include "MakoInstance.h"
#include "MakoClass.h"
#include "Error/ErrorReporter.h"

MakoInstance::MakoInstance(std::shared_ptr<MakoClass> klass)
    : klass(std::move(klass)) {}

std::string MakoInstance::ToString() const {
    return klass->name + " instance";
}

std::any MakoInstance::Get(const Token &name){
    auto it = fields.find(name.Lexeme);
    if (it != fields.end()){
        return it->second;
    }
    
    if (const MakoFunction* method = klass->FindMethod(name.Lexeme)){
        std::shared_ptr<MakoCallable> bound = method->Bind(shared_from_this());
        return bound;
    }

    throw ErrorReporter::RuntimeError(name, "Unidentified property '" + name.Lexeme + "'.");
}

void MakoInstance::Set(const Token& name, std::any value){
    fields.insert_or_assign(name.Lexeme, std::move(value));
}
