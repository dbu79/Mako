#include "../Lexer/Token.h"
#include "ExprPtr.h"

class Expr {
    public: 
        virtual ~Expr() = default;
        class Binary;
    };


class Expr::Binary: public Expr {
    private:
        ExprPtr m_Left; 
        Token m_Operator;
        ExprPtr m_Right;
    public:
        Binary(ExprPtr left, Token op, ExprPtr right) : m_Left(std::move(left)), m_Operator(std::move(op)), m_Right(std::move(right)) {};
};
