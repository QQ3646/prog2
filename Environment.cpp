//#include "Environment.h"
#include "Expressions/SubExpressions.h"

Environment::Pair::Pair(int scope_num, std::string id, Expression* expression) : scope_num(scope_num),
                                                                                 id(std::move(id)),
                                                                                 expression(expression) {}

Environment::Environment() {}

Environment::Environment(const Environment& environment) : list(environment.list) {}

Expression* Environment::fromEnv(const std::string& id) {
    for (auto iterator = list.rbegin(); iterator != list.rend(); ++iterator)
        if (iterator->id == id)
            return iterator->expression;
    throw std::runtime_error("Variable is not found in environment.");
}

int Environment::getValue(Expression* expression) {
    try {
        auto* t = dynamic_cast<Val*>(expression);
        return t->get_value();
    }
    catch (std::bad_cast& ex) {
        throw std::invalid_argument(std::string("Invalid expression type! \nExpected: function\nGiven: ")
                                    + typeid(expression).name() + ".");
    }
}

void Environment::addNewPair(int scope_num, const std::string& id, Expression* expression) {
    list.emplace_back(scope_num, id, expression);
    try {
        auto& func = dynamic_cast<Function&>(*expression);
        func.copyLexEnv(this);
    }
    catch (std::bad_cast& ex) {
        // Не повезло, не фортануло
    }
}

void Environment::clearScope(int scope_num) {
    std::vector<Pair> new_vector;
    for (Pair& pair : list)
        if (pair.scope_num != scope_num)
            new_vector.push_back(pair);
        else
            break;
    list = std::move(new_vector);
}

int Environment::getCurrentScope() {
    if (list.empty())
        return 0;
    else
        return list.back().scope_num;
}

int Environment::getNewScope() {
    return list.empty() ? 0 : getCurrentScope() + 1;
}

void Environment::setToStack(Expression* expression) {
    try {
        Set& set = dynamic_cast<Set&>(*expression);
        int index = -1;
        for (int i = 0; i < list.size(); ++i)
            if (list.at(i).id == set.getId())
                index = i;
        if (index == -1)
            addNewPair(getCurrentScope(), set.getId(), set.getEVal());
        else
            list.at(index).expression = set.getEVal()->eval(*this);
    }
    catch (std::bad_cast& ex) {
        std::cout << "ERROR";
    }
}

Environment::~Environment() {
    for (auto& pair : list) {
        delete pair.expression;
    }
}