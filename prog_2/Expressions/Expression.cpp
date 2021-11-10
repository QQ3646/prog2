#include "../String.h"
#include <typeinfo>

class Expression {
public:
    virtual void print(std::ofstream &stream) = 0;

    virtual Expression *get_copy() = 0;

    virtual Expression *derivative(const String &string) = 0;

    virtual int eval(const String &string) = 0;

    virtual Expression *simple() = 0;

    virtual bool operator==(const Expression &e2) {
        if (typeid(*this) != typeid(e2))
            return false;
        return true;
    };

    virtual bool containsVariable() = 0;

    virtual ~Expression() {}
};

class Binary : public Expression {
protected:
    Expression *first, *second;
    char symbol;

    // Абстрактным будет, тк методы simple(), eval(), get_copy(), derivative() являются абстрактными

    Binary(Expression *first, Expression *second, char symbol) : first(first), second(second), symbol(symbol) {}

    void print(std::ofstream &stream) {
        stream << '(';
        first->print(stream);
        stream << symbol;
        second->print(stream);
        stream << ')';
    }

    bool containsVariable() {
        return first->containsVariable() || second->containsVariable();
    }

    virtual bool operator==(const Binary &op) {
        if (*this->first == *op.first && *this->second == *op.second)
            return true;
        return false;
    }

    virtual bool operator!=(const Binary &op) { // Вот это я придумал, да
        if (*this->first == *op.second && *this->second == *op.first)
            return true;
        return false;
    }

    ~Binary() { // Не очень понимаю зачем здесь виртуальность. У меня же у всех дочерних классов логика удаления одинакова.
        delete first;
        delete second;
    }
};
