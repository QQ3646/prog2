#include "Expression.cpp"

class Number : public Expression {
    int value;

public:
    Number(int value);

    Expression *get_copy();

    Expression *derivative(const String &a);

    void print(std::ofstream &stream);

    int eval(const String &a);

    int get_value() const;

    Expression *simple();

    bool operator==(const Expression &e2);

    bool containsVariable();
};

class Variable : public Expression {
    String name;
public:
    Variable(const String &string);

    Expression *get_copy();

    Expression *derivative(const String &a);

    void print(std::ofstream &stream);

    const String &get_name();

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);

    bool containsVariable();
};

class Add : public Binary {
public:
    Add(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};

class Sub : public Binary {
public:
    Sub(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};

class Mul : public Binary {
public:
    Mul(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};

class Div : public Binary {
public:
    Div(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};