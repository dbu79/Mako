#include "MakoInstance.h"
#include "MakoClass.h"

MakoInstance::MakoInstance(std::shared_ptr<MakoClass> klass)
    : klass(std::move(klass)) {}

std::string MakoInstance::ToString() const {
    return klass->name + " instance";
}
