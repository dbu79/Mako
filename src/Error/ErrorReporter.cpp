#include <iostream>
#include "ErrorReporter.h"
#include "../Lexer/TokenType.h"

bool ErrorReporter::m_HadError = false;
bool ErrorReporter::m_HadRuntimeError = false;

void ErrorReporter::Error(size_t line, const std::string& message){
    Report(line, "", message);
}

ErrorReporter::ParseError ErrorReporter::Error(const Token& token, const std::string& message){
    if (token.Type == TokenType::END_OF_FILE){
        Report(token.Line, " at end", message);
    } else {
        Report(token.Line, " at '" + token.Lexeme + "'", message);
    }

    return ParseError();
}

void ErrorReporter::Report(size_t line, const std::string& where, const std::string& message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    m_HadError = true;
}

void ErrorReporter::ReportRuntimeError(const RuntimeError &error){
    std::cerr << error.what() << "\n[line " << error.token.Line << "]" << "\n"; 
    m_HadRuntimeError = true;
}

bool ErrorReporter::HadError(){
    return m_HadError;
}

bool ErrorReporter::HadRuntimeError(){
    return m_HadRuntimeError;
}
void ErrorReporter::Reset(){
    m_HadError = false;
    m_HadRuntimeError = false;
}
