#pragma once

#include "../Parser/Expr.h"

class Interpreter :  Expr::Visitor {
    public:
        std::any VisitLiteralExpr(const Expr::Literal& expr) override;
};
