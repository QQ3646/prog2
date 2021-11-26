#include "Expression.h"
#include <iostream>
#include <string>
#include <vector>

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

    virtual ~Add();

};

class If : public Expression {
    Expression *e1, *e2;
    Expression *e_then, *e_else;
public:
    If(Expression *e1, Expression *e2, Expression *eThen, Expression *eElse);

    void print() override;

    Expression *eval(Environment &env) override;

    virtual ~If();
};

class Let : public Expression {
    std::string id;
    Expression *e_value, *e_body;
public:
    Let(std::string id, Expression *eValue, Expression *e_body);

    void print() override;

    Expression *eval(Environment &env) override;

    ~Let();
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

    ~Function() override;
};

class Call : public Expression{
    Expression *f_expr, *arg_expr;
public:
    Call(Expression *fExpr, Expression *argExpr);

    Expression *eval(Environment &env) override;

    void print() override;

    ~Call() override;
};

class Set : public Expression {
    std::string id;
    Expression *e_val;
public:
    Set(std::string id, Expression *e_val);

    Expression *eval(Environment &env) override;

    void print() override;

    const std::string &getId() const;

    Expression *getEVal() const;

    ~Set() override;
};

class Block : public Expression {
    std::vector<Expression *> exp_list;
public:
    Block(std::vector<Expression *> &vector);

    Expression *eval(Environment &env) override;

    void print() override;

    ~Block() override;
};