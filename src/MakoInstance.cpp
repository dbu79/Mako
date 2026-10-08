#include "MakoInstance.h"
#include "MakoClass.h"
#include "src/Error/ErrorReporter.h"

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

    throw ErrorReporter::RuntimeError(name, "Unidentified property '" + name.Lexeme + "'.");
}
