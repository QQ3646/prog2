//#include "Expressions/SubExpressions.h"
#include <typeinfo>
#include <iostream>
#include <vector>

class Expression;

class Environment {
    class Pair {
        int scope_num;
        std::string id;
        Expression *expression;

        friend class Function;
        friend class Environment;
    public:
        Pair(int scope_num, std::string id, Expression *expression);
    };

    friend class Function;

    std::vector<Pair> list;
public:
    Environment();

    Environment(const Environment &environment);

    Expression *fromEnv(const std::string &id);

    static int getValue(Expression *expression);

    void addNewPair(int scope_num, const std::string &id, Expression *expression);

    void clearScope(int scope_num);

    int getCurrentScope();

    int getNewScope();

    void setToStack(Expression *expression);

    ~Environment();
};

