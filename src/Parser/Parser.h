#include "../Lexer/Token.h"
#include <string>
#include <vector>

class Parser {
    private:
        const std::vector<Token> Tokens; 
        size_t m_Current = 0;
        
        std::string Expression();
        

    public:
        Parser(const std::vector<Token>& tokens) : Tokens(tokens) {};
};

