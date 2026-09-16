#include <string>

class ErrorReporter {
    public:
        static void Error(size_t line, const std::string& message);
        static bool HadError();
        static void Reset();

    private:
        static void Report(size_t line, const std::string& where, const std::string& message);
        static bool m_HadError;
};
