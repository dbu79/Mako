#include <iostream>
#include "ErrorReporter.h"

bool ErrorReporter::m_HadError = false;

void ErrorReporter::Error(size_t line, const std::string& message){
    Report(line, "", message);
}

void ErrorReporter::Report(size_t line, const std::string& where, const std::string& message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    m_HadError = true;
}


bool ErrorReporter::HadError(){
    return m_HadError;
}
void ErrorReporter::Reset(){
    m_HadError = false;
}
