#include "MakoClass.h"
#include "MakoInstance.h"

MakoClass::MakoClass(std::string name) : name(std::move(name)) {}

std::string MakoClass::ToString() const { return name; }

std::any MakoClass::Call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    return std::make_shared<MakoInstance>(shared_from_this());
}

size_t MakoClass::Arity() const { return 0; }
