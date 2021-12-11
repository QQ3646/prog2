#include "Expression.h"
#include <iostream>
#include <string>
#include <vector>

class Val : public Expression {
    int value;

    void print(std::ostream &os) const override;
public:
    explicit Val(int value);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

    int get_value() const;
};

class Var : public Expression {
    std::string id;

    void print(std::ostream &os) const override;
public:
    explicit Var(std::string id);

    Expression *copy() override;

    Expression *eval(Environment &env) override;
};

class Add : public Expression {
    Expression *e1, *e2;

    void print(std::ostream &os) const override;
public:
    Add(Expression *e1, Expression *e2);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

     bool syntaxCheck() override;

    virtual ~Add();
};

class If : public Expression {
    Expression *e1, *e2;
    Expression *e_then, *e_else;

    void print(std::ostream &os) const override;
public:
    If(Expression *e1, Expression *e2, Expression *eThen, Expression *eElse);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

     bool syntaxCheck() override;

    virtual ~If();
};

class Let : public Expression {
    std::string id;
    Expression *e_value, *e_body;

    void print(std::ostream &os) const override;
public:
    Let(std::string id, Expression *eValue, Expression *e_body);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

     bool syntaxCheck() override;

    ~Let() override;
};

class Function : public Expression {
    std::string id;
    Expression *f_body;

    Environment *lexicalEnv;  // Удобно, если было бы по значению, но к этому времени компиляции Environment незаконченный тип
    // Не знаю, у меня и так зависимости убогие, но не знаю как лучше сделать

    friend class Call;

    void print(std::ostream &os) const override;
public:
    Function(std::string id, Expression *fBody);

    Function(const Function &function);

    Expression *copy() override;

    Expression * eval(Environment &env) override;

    std::string &getId();

    Expression *getFBody();

    void copyLexEnv(Environment *lexicalEnv);

     bool syntaxCheck() override;

    ~Function() override;
};

class Call : public Expression{
    Expression *f_expr, *arg_expr;

    void print(std::ostream &os) const override;
public:
    Call(Expression *fExpr, Expression *argExpr);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

    bool syntaxCheck() override;

    ~Call() override;
};

class Set : public Expression {
    std::string id;
    Expression *e_val;

    void print(std::ostream &os) const override;
public:
    Set(std::string id, Expression *e_val);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

    const std::string &getId() const;

    Expression *getEVal() const;

    bool syntaxCheck() override;

    ~Set() override;
};

class Block : public Expression {
    std::vector<Expression *> exp_list;

    void print(std::ostream &os) const override;
public:
    Block(std::vector<Expression *> vector);

    Expression *copy() override;

    Expression *eval(Environment &env) override;

    bool syntaxCheck() override;

    ~Block() override;
};