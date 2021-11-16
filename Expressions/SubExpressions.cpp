#include "../Environment.h"

// Val {
Val::Val(int value) : value(value) {}

Expression *Val::eval(Environment &env) {
    return this;
}

void Val::print() {
    std::cout << "(val " << value << ")";
}

int Val::get_value() const { return value; }
// Val }


// Var {
Var::Var(std::string id) : id(std::move(id)) {}

Expression *Var::eval(Environment &env) {
    return env.fromEnv(id);
}

void Var::print() {
    std::cout << "(var " << id << ")";
}

std::string Var::get_id() {
    return id;
}
// Var }


// Add {
Add::Add(Expression *e1, Expression *e2) : e1(e1), e2(e2) {}

void Add::print() {
    std::cout << "(add ";
    e1->print();
    std::cout << " ";
    e2->print();
    std::cout << ")";
}

Expression *Add::eval(Environment &env) {
    return new Val(env.getValue(e1->eval(env)) + env.getValue(e2->eval(env)));
}
// Add }


// If {
If::If(Expression *e1, Expression *e2, Expression *eThen, Expression *eElse) : e1(e1), e2(e2), e_then(eThen),
                                                                               e_else(eElse) {}

void If::print() {
    std::cout << "(if ";
    e1->print();
    std::cout << " ";
    e2->print();
    std::cout << " then ";
    e_then->print();
    std::cout << " else ";
    e_else->print();
    std::cout << ")";
}

Expression *If::eval(Environment &env) {
    if (env.getValue(e1->eval(env)) > env.getValue(e2->eval(env)))
        return e_then->eval(env);
    else
        return e_else->eval(env);
}
// If }


// Let {
Let::Let(std::string id, Expression *eValue, Expression *e_body) : id(std::move(id)), e_value(eValue), e_body(e_body) {}

void Let::print() {
    std::cout << "(let " << id << " = ";
    e_value->print();
    std::cout << " in ";
    e_body->print();
    std::cout << ")";
}

Expression *Let::eval(Environment &env) {
    int scope_num = env.getNewScope();
    env.addNewPair(scope_num, id, e_value->eval(env));
    auto *exp = e_body->eval(env);
    env.clearScope(scope_num);
    return exp;
}
// Let }


// Function {
Function::Function(std::string &id, Expression *fBody) : id(std::move(id)), f_body(fBody) {}

void Function::print() {
    std::cout << "(function " << id;
    f_body->print();
    std::cout << ")";
}

Expression *Function::eval(Environment &env) {
    return this;
}

std::string &Function::getId() {
    return id;
}

Expression *Function::getFBody() {
    return f_body;
}
// Function }


// Call {
Call::Call(Expression *fExpr, Expression *argExpr) : f_expr(fExpr), arg_expr(argExpr) {}

Expression *Call::eval(Environment &env) {
    try {
        auto &function = dynamic_cast<Function &>(*f_expr->eval(env));
        int scope_num = env.getNewScope();
        env.addNewPair(scope_num, function.getId(), arg_expr->eval(env));
        auto *exp = function.getFBody()->eval(env);
        env.clearScope(scope_num);
        return exp;
    } catch (std::bad_cast &ex) {}
    return nullptr;
}

void Call::print() {

}
// Call }
