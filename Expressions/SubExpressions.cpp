#include <algorithm>
#include "SubExpressions.h"

Val::Val(int value) : value(value) {}

Expression* Val::copy() {
    return new Val(value);
}

Expression* Val::eval(Environment& env) {
    return copy();
}

void Val::print(std::ostream& os) const {
    os << "(val " + std::to_string(value) + ")";
}

int Val::get_value() const { return value; }
// Val }


// Var {
Var::Var(std::string id) : id(std::move(id)) {}

Expression* Var::copy() {
    return new Var(id);
}

Expression* Var::eval(Environment& env) {
    return env.fromEnv(id);
}

void Var::print(std::ostream& os) const {
    os << "(var " + id + ")";
}
// Var }


// Add {
Add::Add(Expression* e1, Expression* e2) : e1(e1), e2(e2) {}

Expression* Add::copy() {
    return new Add(e1->copy(), e2->copy());
}

void Add::print(std::ostream& os) const {
    os << "(add " << e1 << " " << e2 << ")";
}

Expression* Add::eval(Environment& env) {
    return new Val(env.getValue(e1->eval(env)) + env.getValue(e2->eval(env)));
}

bool Add::syntaxCheck() {
    return (e1 && e2) && e1->syntaxCheck() && e2->syntaxCheck();
}

Add::~Add() {
    delete e1;
    delete e2;
}
// Add }


// If {
If::If(Expression* e1, Expression* e2, Expression* eThen, Expression* eElse) : e1(e1), e2(e2), e_then(eThen),
                                                                               e_else(eElse) {}

Expression* If::copy() {
    return new If(e1->copy(), e2->copy(),
                  e_then->copy(), e_else->copy());
}

void If::print(std::ostream& os) const {
    os << "(if " << e1 << " " << e2 << " then " << e_then << " else " << e_else << ")";
}

Expression* If::eval(Environment& env) {
    if (env.getValue(e1->eval(env)) > env.getValue(e2->eval(env)))
        return e_then->eval(env);
    else
        return e_else->eval(env);
}

bool If::syntaxCheck() {
    return (e1 && e2 && e_then && e_else) && e1->syntaxCheck() && e2->syntaxCheck() &&
           e_then->syntaxCheck() && e_else->syntaxCheck();
}

If::~If() {
    delete e1;
    delete e2;
    delete e_then;
    delete e_else;
}
// If }


// Let {
Let::Let(std::string id, Expression* eValue, Expression* e_body) : id(std::move(id)), e_value(eValue), e_body(e_body) {}

Expression* Let::copy() {
    return new Let(id, e_value->copy(), e_body->copy());
}

void Let::print(std::ostream& os) const {
    os << "(let " + id + " = " << e_value << " in " << e_body << ")";
}

Expression* Let::eval(Environment& env) {
    int scope_num = env.getNewScope();
    env.addNewPair(scope_num, id, e_value->eval(env));
    auto* exp = e_body->eval(env);
    env.clearScope(scope_num);
    return exp;
}

bool Let::syntaxCheck() {
    return (e_value && e_body) && e_value->syntaxCheck() && e_body->syntaxCheck();
}

Let::~Let() {
    delete e_value;
    delete e_body;
}
// Let }


// Function {
Function::Function(std::string id, Expression* fBody) : id(std::move(id)), f_body(fBody), lexicalEnv(nullptr) {}

Function::Function(const Function& function) : id(function.id), f_body(function.f_body->copy()) {
    if (function.lexicalEnv) {
        *lexicalEnv = *function.lexicalEnv;
        for (auto& elem : lexicalEnv->list) {
            if (elem.expression != this)
                elem.expression = elem.expression->copy();
            else
                elem.expression = this;
        }
    }
}

Expression* Function::copy() {
    return new Function(id, f_body->copy());
}

void Function::print(std::ostream& os) const {
    os << "(function " + id + " " << f_body << ")";
}

Expression* Function::eval(Environment& env) {
    return new Function(*this);
}

std::string& Function::getId() {
    return id;
}

void Function::copyLexEnv(Environment* lexEnv) {
    lexicalEnv = new Environment(*lexEnv);
    for (auto& elem : lexicalEnv->list) {
        if (elem.expression != this)
            elem.expression = elem.expression->copy();
    }
}

Expression* Function::getFBody() {
    return f_body;
}

bool Function::syntaxCheck() {
    return !id.empty() && f_body && f_body->syntaxCheck();
}

Function::~Function() {
    delete f_body;
    if (lexicalEnv)
        for (auto& trio : lexicalEnv->list)
            if (trio.expression == this)
                trio.expression = nullptr;
    delete lexicalEnv;
}
// Function }


// Call {
Call::Call(Expression* fExpr, Expression* argExpr) : f_expr(fExpr), arg_expr(argExpr) {}

Expression* Call::copy() {
    return new Call(f_expr->copy(), arg_expr->copy());
}

Expression* Call::eval(Environment& env) {
    try {
        auto& function = dynamic_cast<Function&>(*f_expr->eval(env));
        int scope_num = function.lexicalEnv->getNewScope();
        function.lexicalEnv->addNewPair(scope_num, function.getId(), arg_expr->eval(env));
        auto* exp = function.getFBody()->eval(*function.lexicalEnv);
        function.lexicalEnv->clearScope(scope_num);
        return exp;
    }
    catch (std::bad_cast& ex) {
        throw std::invalid_argument(std::string("Invalid expression type! \nExpected: function\nGiven: ")
                                    + typeid(f_expr).name() + ".");
    }
}

void Call::print(std::ostream& os) const {
    os << "(call " << f_expr << " " << arg_expr << ")";
}

bool Call::syntaxCheck() {
    return (f_expr && arg_expr) && f_expr->syntaxCheck() && arg_expr->syntaxCheck();
}

Call::~Call() {
    delete f_expr;
    delete arg_expr;
}
// Call }

// Set {
Set::Set(std::string id, Expression* e_val) : id(std::move(id)), e_val(e_val) {}

Expression* Set::copy() {
    return new Set(id, e_val->copy());
}

Expression* Set::eval(Environment& env) {
    env.setToStack(this);
    return this;
}

void Set::print(std::ostream& os) const {
    os << "(set " << e_val << ")";
}

const std::string& Set::getId() const {
    return id;
}

Expression* Set::getEVal() const {
    return e_val;
}

bool Set::syntaxCheck() {
    return e_val && e_val->syntaxCheck();
}

Set::~Set() {
    delete e_val;
}
// Set }

// Block {
Block::Block(std::vector<Expression*> vector) : exp_list(std::move(vector)) {}

Expression* Block::copy() {
    std::vector<Expression*> t = exp_list;
    for (auto& elem : t)
        elem = elem->copy();
    return new Block(t);
}

Expression* Block::eval(Environment& env) {
    Expression* return_exp = nullptr;
    for (auto elem : exp_list) {
        return_exp = elem->eval(env);
    }
    return return_exp;
}

void Block::print(std::ostream& os) const {
    os << "(block";
    for (auto expression : exp_list)
        os << " " << expression;
    os << ")";
}

bool Block::syntaxCheck() {
    if (exp_list.empty())
        return false;

    return std::all_of(exp_list.begin(), exp_list.end(), [](Expression* expression) { return expression; });
}

Block::~Block() {
    for (Expression* exp : exp_list) {
        delete exp;
        exp = nullptr;
    }
}
// Block }