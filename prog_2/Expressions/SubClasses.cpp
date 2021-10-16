#include "SubClasses.h"
#include <typeinfo>

enum typeList {
    number, var, add, sub,
};

Number::Number(int value) {
    this->value = value;
}

Expression *Number::get_copy() {
    return new Number(value);
}

Expression *Number::derivative(const String &a) {
    return new Number(0);
}

void Number::print(std::ofstream &stream) {
    stream << value;
}

int Number::eval(const String &a) { return value; }

int Number::get_value() const { return value; }

Expression *Number::simplify() { return this; }


Variable::Variable(const String &string) {
    this->name = string;
}

Expression *Variable::get_copy() {
    return new Variable(name);
}

void Variable::print(std::ofstream &stream) {
    stream << name;
}

Expression *Variable::derivative(const String &string) {
    for (int i = 0; string[i] != '\0' and this->name[i] != '\0'; ++i) {
        if (this->name[i] != string[i])
            return new Number(0);
    }
    return new Number(1);
}

int Variable::eval(const String &string) {
    bool skip = false;
    for (int i = 0; i < string.get_len(); ++i) {
        if (skip && string[i] == ';') {
            skip = true;
            continue;
        }
        if (skip) continue;
        if (isalnum(string[i])) {
            int j = i;
            for(; string[j] != ' '; j++);
            if (string(i, j) == name)
                return 1;
            else
                skip = true;
        }
    }
    return 0;
}

Expression *Variable::simplify() { return nullptr; }

const String &Variable::get_name() { return name; }


Add::Add(Expression *first, Expression *second) : Operations(first, second, '+') {}

Expression *Add::get_copy() {
    return new Add(first->get_copy(), second->get_copy());
}

Expression *Add::derivative(const String &string) {
    return new Add(first->derivative(string), second->derivative(string));
}

int Add::eval(const String &string) { return first->eval(string) + second->eval(string); }

Expression *Add::simplify() { return nullptr; }


Sub::Sub(Expression *first, Expression *second) : Operations(first, second, '-') {}

Expression *Sub::get_copy() {
    return new Sub(first->get_copy(), second->get_copy());
}

Expression *Sub::derivative(const String &string) {
    return new Sub(first->derivative(string), second->derivative(string));
}

int Sub::eval(const String &string) { return first->eval(string) - second->eval(string); }

Expression *Sub::simplify() { return nullptr; }


Mul::Mul(Expression *first, Expression *second) : Operations(first, second, '*') {}

Expression *Mul::get_copy() {
    return new Mul(first->get_copy(), second->get_copy());
}

Expression *Mul::derivative(const String &string) {
    return new Add(new Mul(first->derivative(string), second->get_copy()),
                   new Mul(first->get_copy(), second->derivative(string)));
}

int Mul::eval(const String &string) { return first->eval(string) * second->eval(string); }

Expression *Mul::simplify() {
    first->simplify();
    second->simplify();
    if (typeid(first) == typeid(Number)) {
        Number &cpy = dynamic_cast<Number &>(*first);

    }
    return nullptr;
}


Div::Div(Expression *first, Expression *second) : Operations(first, second, '/') {}

Expression *Div::get_copy() {
    return new Div(first->get_copy(), second->get_copy());
}

Expression *Div::derivative(const String &string) {
    return new Div(new Sub(new Mul(first->derivative(string), second->get_copy()),
                           new Mul(first->get_copy(), second->derivative(string))),
                   new Mul(second->get_copy(), second->get_copy()));
}

int Div::eval(const String &string) { return first->eval(string) / second->eval(string); }

Expression *Div::simplify() {
    if (typeid(first) == typeid(second)) {
        if (typeid(first) == typeid(Number)) {
            if (dynamic_cast<Number &>(*first).get_value() == dynamic_cast<Number &>(*second).get_value())
                return new Number(1);
        } else if (typeid(first) == typeid(Variable)) {
            const String cpy1 = dynamic_cast<Variable &>(*first).get_name();
            const String cpy2 = dynamic_cast<Variable &>(*second).get_name();
            int i = 0;
            for (; cpy1[i] != '\0' and cpy2[i] != '\0'; ++i) {
                if (cpy1[i] != cpy2[i])
                    return this;
            }
            if (cpy2[i] == '\0' and cpy1[i] == '\0')
                return new Number(1);
        }
    }
    return this;
}