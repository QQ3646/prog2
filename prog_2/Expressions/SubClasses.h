#pragma once
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

    bool findX();
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

    bool findX();
};

class Add : public Operations {
public:
    Add(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};

class Sub : public Operations {
public:
    Sub(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};

class Mul : public Operations {
public:
    Mul(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};

class Div : public Operations {
public:
    Div(Expression *first, Expression *second);

    Expression *get_copy();

    Expression *derivative(const String &a);

    int eval(const String &a);

    Expression *simple();

    bool operator==(const Expression &e2);
};