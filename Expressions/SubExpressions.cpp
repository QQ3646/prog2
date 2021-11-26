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
    if (e1 == nullptr || e2 == nullptr) {
        std::cout << "ERROR";
        exit(0);
    }
    return new Val(env.getValue(e1->eval(env)) + env.getValue(e2->eval(env)));
}

Add::~Add() {
    delete e1;
    delete e2;
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

If::~If() {
    delete e1;
    delete e2;
    delete e_then;
    delete e_else;
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
    if (e_value == nullptr || e_body == nullptr) {
        std::cout << "ERROR";
        exit(0);
    }
    int scope_num = env.getNewScope();
    env.addNewPair(scope_num, id, e_value->eval(env));
    auto *exp = e_body->eval(env);
    env.clearScope(scope_num);
    return exp;
}

Let::~Let() {
    delete e_value;
    delete e_body;
}
// Let }


// Function {
Function::Function(std::string &id, Expression *fBody) : id(std::move(id)), f_body(fBody) {}

void Function::print() {
    std::cout << "(function " << id << " ";
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
    if (f_body == nullptr){
            std::cout << "ERROR";
            exit(0);
    }
    return f_body;
}

Function::~Function() {
    delete f_body;
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
    std::cout << "(call ";
    f_expr->print();
    std::cout << " ";
    arg_expr->print();
    std::cout << ")";
}

Call::~Call() {
    delete f_expr;
    delete arg_expr;
}
// Call }

// Set {
Set::Set(std::string id, Expression *e_val) : id(std::move(id)), e_val(e_val){}

Expression *Set::eval(Environment &env) {
    env.setToStack(this);
    return this;
}

void Set::print() {
    std::cout << "(set " << id << " ";
    e_val->print();
    std::cout << ")";
}

const std::string &Set::getId() const {
    return id;
}

Expression *Set::getEVal() const {
    return e_val;
}

Set::~Set() {
    delete e_val;
}
// Set }

// Block {
Block::Block(std::vector<Expression *> &vector) : exp_list(std::move(vector)) { }

Expression *Block::eval(Environment &env) {
    Expression *return_exp;
    for (auto elem: exp_list) {
        return_exp = elem->eval(env);
    }
    return return_exp;
}

void Block::print() {
    std::cout << "(block";
    for (auto expression: exp_list) {
        std::cout << " ";
        expression->print();
    }
    std::cout << ")";
}

Block::~Block() {
    for (Expression *exp: exp_list) {
        delete exp;
    }
}
// Block }


