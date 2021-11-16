#include "Expression.h"
#include <iostream>

class Val : public Expression {
    int value;
public:
    explicit Val(int value);

    Expression *eval(Environment &env) override;

    void print() override;

    int get_value() const;
};

class Var : public Expression {
    std::string id;
public:
    explicit Var(std::string id);

    Expression *eval(Environment &env) override;

    void print() override;

    std::string get_id();
};

class Add : public Expression {
    Expression *e1, *e2;
public:
    Add(Expression *e1, Expression *e2);

    void print() override;

    Expression *eval(Environment &env) override;

};

class If : public Expression {
    Expression *e1, *e2;
    Expression *e_then, *e_else;
public:
    If(Expression *e1, Expression *e2, Expression *eThen, Expression *eElse);

    void print() override;

    Expression *eval(Environment &env) override;
};

class Let : public Expression {
    std::string id;
    Expression *e_value, *e_body;
public:
    Let(std::string id, Expression *eValue, Expression *e_body);

    void print() override;

    Expression *eval(Environment &env) override;
};

class Function : public Expression {
    std::string id;
    Expression *f_body;
public:
    Function(std::string &id, Expression *fBody);

    void print() override;

    Expression * eval(Environment &env) override;

    std::string &getId();

    Expression *getFBody();
};

class Call : public Expression{
    Expression *f_expr, *arg_expr;
public:
    Call(Expression *fExpr, Expression *argExpr);

    Expression *eval(Environment &env) override;

    void print() override;
};