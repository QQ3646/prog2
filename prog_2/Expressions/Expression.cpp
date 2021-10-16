#include "../String.h"

class Expression {
public:
    virtual void print(std::ofstream &stream) = 0;

    virtual Expression *get_copy() = 0;

    virtual Expression *derivative(const String &string) = 0;

    virtual int eval(const String &string) = 0;

    virtual Expression *simplify() = 0;

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

    virtual int eval(const String &string) = 0; // Чтобы сделать метод абстрактным (да, костыль)

    void print(std::ofstream &stream) {
        stream << '(';
        first->print(stream);
        stream << symbol;
        second->print(stream);
        stream << ')';
    };

    ~Operations() {
        delete first;
        delete second;
    }
};
