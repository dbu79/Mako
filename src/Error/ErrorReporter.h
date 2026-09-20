#include "../Lexer/Token.h"
#include <stdexcept>
#include <string>

class ErrorReporter {
    public:
        class ParseError : public std::runtime_error{
            public:
                ParseError() : std::runtime_error("Parse error"){}
        };

        static void Error(size_t line, const std::string& message);
        static bool HadError();
        static void Reset();
        static ParseError Error(const Token& token, const std::string& message);

    private:
        static void Report(size_t line, const std::string& where, const std::string& message);
        static bool m_HadError;
};
