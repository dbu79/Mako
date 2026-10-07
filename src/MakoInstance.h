#pragma once

#include <string>
#include <memory>

class MakoClass;

class MakoInstance {
    public: 
        explicit MakoInstance(std::shared_ptr<MakoClass> klass);

        std::string ToString() const;

    private: 
        std::shared_ptr<MakoClass> klass;
};
