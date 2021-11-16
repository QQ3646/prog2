#include "Environment.h"

Environment::Pair::Pair(int scope_num, std::string &id, Expression *expression) : scope_num(scope_num), id(std::move(id)), expression(expression) {}

Expression *Environment::fromEnv(std::string &id) {
    for (Pair &pair: list)
        if (pair.id == id)
            return pair.expression;
    std::cout << "ERROR";
    exit(0);
}

int Environment::getValue(Expression *expression) {
    try {
        auto &t = dynamic_cast<Val &>(*expression);
        return t.get_value();
    } catch (std::bad_cast &ex) {}
    std::cout << "ERROR";
    exit(0);
}

void Environment::addNewPair(int scope_num, std::string &id, Expression *expression) {
    list.emplace_back(scope_num, id, expression);
}

void Environment::clearScope(int scope_num) {
    std::vector<Pair> new_vector;
    for (Pair &pair: list)
        if (pair.scope_num != scope_num)
            new_vector.push_back(pair);
        else
            break;
    list = std::move(new_vector);
}

int Environment::getNewScope() {
    if (list.empty())
        return 0;
    else
        return list.back().scope_num + 1;
}
