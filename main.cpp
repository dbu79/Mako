#include <fstream>
#include <iostream>
#include <sstream>

#include "src/Lexer/Lexer.h"
#include "src/Parser/Parser.h"
#include "src/Error/ErrorReporter.h"

void Run(std::string source){
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.ScanTokens();

    // for (const auto& token : tokens){
    //     std::cout << token.ToString() << "\n";
    // }
    
    Parser parser(tokens);
    ExprPtr expression = parser.Parse();
    if (ErrorReporter::HadError()) return;

}

void RunFile(std::string path){
    std::ifstream file(path);
    if (!file){
        std::cout << "Could not open file: " << path << "\n";
        std::exit(65);
    } 

    std::ostringstream buffer;
    buffer << file.rdbuf();
    Run(buffer.str());

};
void RunPrompt(){
    std::string input;
    while (true){
        std::cout << "> ";
        if (!std::getline(std::cin, input)){
            std::cout << "\n";
            break;
        }
        if (input.empty()) continue;

        Run(input);
    }
};

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

