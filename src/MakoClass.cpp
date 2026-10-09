#include "MakoClass.h"
#include "MakoInstance.h"

MakoClass::MakoClass(std::string name,std::unordered_map<std::string, MakoFunction> methods) 
    : name(std::move(name)), methods(std::move(methods)) {}


const MakoFunction* MakoClass::FindMethod(const std::string &name){
    auto it = methods.find(name);
    if (it != methods.end()){
        return &it->second;
    }
    return nullptr;
}

std::string MakoClass::ToString() const { return name; }

std::any MakoClass::Call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    return std::make_shared<MakoInstance>(shared_from_this());
}

size_t MakoClass::Arity() const { return 0; }
