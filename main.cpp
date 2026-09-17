#include <iostream>

#include "src/Lexer/Lexer.h"

void Run(std::string source){
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.ScanTokens();

    for (const auto& token : tokens){
        std::cout << token.ToString() << "\n";
    }

}

void RunFile(std::string path){

};
void RunPrompt(){};

int main(int argc, char* argv[]){
    if (argc > 2){
        std::cerr << "Usage: mako [script]\n";
        return 64;
    } else if (argc == 2 ){
        RunFile(argv[1]);
    } else {
        RunPrompt();
    }
}
