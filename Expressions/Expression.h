#include <iostream>
#include "../Environment.h"

class Expression {
    virtual void print(std::ostream &os) const = 0;

public:
    virtual Expression *eval(Environment &env) = 0;

    virtual ~Expression() = default;

    virtual Expression *copy() = 0;

    virtual bool syntaxCheck() { return true; }

    friend std::ostream &operator<<(std::ostream &os, const Expression *expression) {
        if (expression)
            expression->print(os);
        return os;
    }
};
