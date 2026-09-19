#include <iostream>
#include <string>

class GenerateAST {
    public:
        static int main(int argc, char* argv[]) {
            if (argc != 1){
                std::cerr << "Usage: generate_ast <output directory>";
                return 64;
            };

            std::string OutputDir = argv[0];
        };
};
