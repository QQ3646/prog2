//#pragma once
//#include "../Environment.h"
class Environment;

class Expression {
public:
    virtual Expression *eval(Environment &env) = 0;

    virtual void print() = 0;
};
