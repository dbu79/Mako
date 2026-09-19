#include "../Lexer/Token.h"


class Expr {
    public: 
        virtual ~Expr() = default;
        class Binary;
    };

class Expr::Binary: public Expr {
    private:
        Expr m_Left; 
        Token m_Operator;
        Expr m_Right;
    public:
        Binary(Expr m_Left, Token m_Operator, Expr m_Right) : m_Left(m_Left), m_Operator(m_Operator), m_Right(m_Right) {};
};
