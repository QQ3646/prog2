#include "SubClasses.h"


Number::Number(int value) : value(value) {}

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

Expression *Number::simple() { return this->get_copy(); }

bool Number::operator==(const Expression &e2) {
    try {
        auto &tempe2 = dynamic_cast<const Number &>(e2);
        if (tempe2.value == value)
            return true;
    } catch (std::bad_cast &ex) {}
    return false;
}

bool Number::containsVariable() {
    return false;
}


Variable::Variable(const String &string) : name(string) {}

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
            for (; string[j] != ' '; j++);
            if (string(i, j) == name)
                return 1;
            else
                skip = true;
        }
    }
    return 0;
}

Expression *Variable::simple() { return this->get_copy(); }

const String &Variable::get_name() { return name; }

bool Variable::operator==(const Expression &e2) {
    try {
        auto &tempe2 = const_cast<Variable&>(dynamic_cast<const Variable &>(e2));
        if (tempe2.name == this->name)
            return true;
    } catch (std::bad_cast &ex) {}
    return false;
}

bool Variable::containsVariable() {
    return true;
}


Add::Add(Expression *first, Expression *second) : Binary(first, second, '+') {}

Expression *Add::get_copy() {
    return new Add(first->get_copy(), second->get_copy());
}

Expression *Add::derivative(const String &string) {
    return new Add(first->derivative(string), second->derivative(string));
}

int Add::eval(const String &string) { return first->eval(string) + second->eval(string); }

Expression *Add::simple() {
    Expression *s_e1 = this->first->simple();
    Expression *s_e2 = this->second->simple();
    Add *temp = new Add(s_e1, s_e2);
    if (!temp->containsVariable()) {
        int eval_res = temp->eval("");
        delete temp;
        return new Number(eval_res);
    }
    return temp;
}

bool Add::operator==(const Expression &e2) {
    try {
        auto &tempe2 = const_cast<Add &>(dynamic_cast<const Add &>(e2));
        if (Binary::operator==(tempe2) || Binary::operator!=(tempe2))
            return true;
    } catch (std::bad_cast &ex) {}
    return false;
}


Sub::Sub(Expression *first, Expression *second) : Binary(first, second, '-') {}

Expression *Sub::get_copy() {
    return new Sub(first->get_copy(), second->get_copy());
}

Expression *Sub::derivative(const String &string) {
    return new Sub(first->derivative(string), second->derivative(string));
}

int Sub::eval(const String &string) { return first->eval(string) - second->eval(string); }

Expression *Sub::simple() {
    Expression *s_e1 = this->first->simple();
    Expression *s_e2 = this->second->simple();
    if (*s_e1 == *s_e2) {
        delete s_e1;
        delete s_e2;

        return new Number(0);
    }
    Sub *temp = new Sub(s_e1, s_e2);
    if (!temp->containsVariable()) {
        int eval_res = temp->eval("");

        delete temp;

        return new Number(eval_res);
    }
    return temp;
}

bool Sub::operator==(const Expression &e2) {
    try {
        auto &tempe2 = const_cast<Sub &>(dynamic_cast<const Sub &>(e2));
        if (Binary::operator==(tempe2))
            return true;
    } catch (std::bad_cast &ex) {}
    return false;
}


Mul::Mul(Expression *first, Expression *second) : Binary(first, second, '*') {}

Expression *Mul::get_copy() {
    return new Mul(first->get_copy(), second->get_copy());
}

Expression *Mul::derivative(const String &string) {
    return new Add(new Mul(first->derivative(string), second->get_copy()),
                   new Mul(first->get_copy(), second->derivative(string)));
}

int Mul::eval(const String &string) { return first->eval(string) * second->eval(string); }

bool Mul::operator==(const Expression &e2) {
    try {
        auto &tempe2 = const_cast<Mul &>(dynamic_cast<const Mul &>(e2));
        if (Binary::operator==(tempe2) || Binary::operator!=(tempe2))
            return true;
    } catch (std::bad_cast &ex) {}
    return false;
}

Expression *Mul::simple() {
    Expression *s_e1 = this->first->simple();
    Expression *s_e2 = this->second->simple();

    for (int i = 0; i < 2; ++i) {
        if (typeid(*s_e1) == typeid(Number)) {
            auto &temp = dynamic_cast<Number &>(*s_e1);
            if (temp.get_value() == 0) {
                delete s_e1;
                delete s_e2;

                return new Number(0);
            } else if (temp.get_value() == 1) {
                delete s_e1;
                if (!s_e2->containsVariable()) {
                    int eval_res2 = s_e2->eval("");
                    delete s_e2;
                    return new Number(eval_res2);
                }
                return s_e2;
            }
        }
        Expression *t_s = s_e1;
        s_e1 = s_e2;
        s_e2 = t_s;
    }
    Mul *temp = new Mul(s_e1, s_e2);
    if (!temp->containsVariable()) {
        int eval_res = temp->eval("");
        delete temp;
        return new Number(eval_res);
    }
    return temp;
}


Div::Div(Expression *first, Expression *second) : Binary(first, second, '/') {}

Expression *Div::get_copy() {
    return new Div(first->get_copy(), second->get_copy());
}

Expression *Div::derivative(const String &string) {
    return new Div(new Sub(new Mul(first->derivative(string), second->get_copy()),
                           new Mul(first->get_copy(), second->derivative(string))),
                   new Mul(second->get_copy(), second->get_copy()));
}

int Div::eval(const String &string) { return first->eval(string) / second->eval(string); }

bool Div::operator==(const Expression &e2) {
    try {
        auto &tempe2 = const_cast<Div &>(dynamic_cast<const Div &>(e2));
        if (Binary::operator==(tempe2))
            return true;
    } catch (std::bad_cast &ex) {}
    return false;
}

Expression *Div::simple() {
    Expression *s_e1 = this->first->simple();
    Expression *s_e2 = this->second->simple();
    if (*s_e1 == *s_e2) {
        delete s_e1;
        delete s_e2;

        return new Number(1);
    }
    Div *temp = new Div(s_e1, s_e2);
    if (!temp->containsVariable()) {
        int eval_res = temp->eval("");
        delete temp;
        return new Number(eval_res);
    }
    return temp;
}
