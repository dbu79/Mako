#include "MakoClass.h"
#include "MakoInstance.h"

MakoClass::MakoClass(std::string name, std::shared_ptr<MakoClass> superclass, std::unordered_map<std::string, MakoFunction> methods) 
    : name(std::move(name)), superclass(std::move(superclass)), methods(std::move(methods)) {}


const MakoFunction* MakoClass::FindMethod(const std::string &name) const {
    auto it = methods.find(name);
    if (it != methods.end()){
        return &it->second;
    }
    
    if (superclass != nullptr){
        return superclass->FindMethod(name);
    }

    return nullptr;
}

std::string MakoClass::ToString() const { return name; }

std::any MakoClass::Call(Interpreter& interpreter, const std::vector<std::any>& arguments) {
    auto instance = std::make_shared<MakoInstance>(shared_from_this());
    const MakoFunction* initializer = FindMethod("init");
    if (initializer != nullptr){
        initializer->Bind(instance)->Call(interpreter, arguments);
    }
    return instance;
}

size_t MakoClass::Arity() const { 
    const MakoFunction* initializer = FindMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->Arity();
}
