#include "Expressions/SubExpressions.h"
#include <typeinfo>

class Environment {
    class Pair {
        int scope_num;
        std::string id;
        Expression *expression;

        friend class Environment;
    public:
        Pair(int scope_num, std::string id, Expression *expression);
    };

    std::vector<Pair> list;
public:
    Expression *fromEnv(const std::string &id);

    static int getValue(Expression *expression);

    void addNewPair(int scope_num, const std::string &id, Expression *expression);

    void clearScope(int scope_num);

    int getCurrentScope();

    int getNewScope();

    void setToStack(Expression *expression);
};
