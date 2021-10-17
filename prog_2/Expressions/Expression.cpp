#include "../String.h"

class Expression {
public:
    virtual void print(std::ofstream &stream) = 0;

    virtual Expression *get_copy() = 0;

    virtual Expression *derivative(const String &string) = 0;

    virtual int eval(const String &string) = 0;

    virtual Expression *simple() = 0;

    virtual bool operator==(const Expression &e2) = 0;

    virtual bool findX() = 0;

    virtual ~Expression() {}
};

class Operations : public Expression {
protected:
    Expression *first, *second;
    char symbol;


    Operations(Expression *first, Expression *second, char symbol) {
        this->first = first;
        this->second = second;
        this->symbol = symbol;
    }

    void print(std::ofstream &stream) {
        stream << '(';
        first->print(stream);
        stream << symbol;
        second->print(stream);
        stream << ')';
    }

    bool findX() {
        return first->findX() || second->findX();
    }

    ~Operations() {
        delete first;
        delete second;
    }
};
