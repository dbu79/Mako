#include "../Lexer/Token.h"
#include <stdexcept>
#include <string>

class ErrorReporter {
    public:
        class ParseError : public std::runtime_error{
            public:
                ParseError() : std::runtime_error("Parse error"){}
        };

        class RuntimeError : public std::runtime_error{
            public:
                RuntimeError(const Token& token, const std::string& message)
                    : std::runtime_error(message), token(token){};
            const Token token;
        };

        static void Error(size_t line, const std::string& message);
        static ParseError Error(const Token& token, const std::string& message);
        static void ReportRuntimeError(const RuntimeError& error);
        static bool HadError();
        static bool HadRuntimeError();
        static void Reset();

    private:
        static void Report(size_t line, const std::string& where, const std::string& message);
        
        static bool m_HadError;
        static bool m_HadRuntimeError;
};
