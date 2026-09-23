#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "src/Lexer/Lexer.h"
#include "src/Parser/Parser.h"
#include "src/Error/ErrorReporter.h"
#include "src/Interpreter/Interpreter.h"

static Interpreter interpreter;

void Run(std::string source){
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.ScanTokens();

    Parser parser(tokens);
    std::vector<StmtPtr> statements = parser.Parse();
    if (ErrorReporter::HadError()) return;

    interpreter.Interpret(statements);
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

    if (ErrorReporter::HadError()) std::exit(65);
    if (ErrorReporter::HadRuntimeError()) std::exit(70);
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
        ErrorReporter::Reset();
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
    return 0;
}

