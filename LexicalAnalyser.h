#include <fstream>
#include "Environment.h"

class LexicalAnalyser {
    std::ifstream &input;
    char current_char;

    void skipSpaces() {
        if (!(current_char == ' ' || current_char == '\n' || current_char == '\t' || current_char == '\r'))
            return;
        while ((current_char = (char) input.get()) == ' ' || current_char == '\n' || current_char == '\t' || current_char == '\r');
    }

    std::string readName() {
        std::string res{};
        skipSpaces();
        do
            res += current_char;
        while (isalnum((current_char = (char) input.get())));
        return res;
    }

    int readValue() {
        int res{};
        skipSpaces();
        bool minus = false;
        if (current_char == '-') {
            minus = true;
            current_char = (char) input.get();
        }
        do
            res = res * 10 + (current_char - '0');
        while (isdigit(((current_char = (char) input.get()))));
        return !minus ? res : -res;
    }

public:
    explicit LexicalAnalyser(std::ifstream &input) : input(input), current_char('\0') {}

    Expression *recognizeExpression() {
        Expression *exp = nullptr;
        while (current_char != ')') {
            if ((current_char = (char) input.get()) == '(') {
                current_char = (char) input.get();
                auto res = readName();

                if (res == "val") {
                    int value = readValue();

                    exp = new Val(value);
                } else if (res == "var") {
                    auto id = readName();

                    exp = new Var(id);
                } else if (res == "add") {
                    auto *e1 = recognizeExpression();
                    auto *e2 = recognizeExpression();

                    exp = new Add(e1, e2);
                } else if (res == "if") {
                    auto *e1 = recognizeExpression();
                    auto *e2 = recognizeExpression();

                    auto temp = readName();
                    if (temp != "then") {
                        delete e1;
                        delete e2;
                        break;
                    }

                    auto *e_then = recognizeExpression();

                    temp = readName();
                    if (temp != "else") { // Наверное как-то по-другому можно проверить ключевые слова...
                        delete e1;
                        delete e2;
                        delete e_then;
                        break;
                    }

                    auto *e_else = recognizeExpression();
                    exp = new If(e1, e2, e_then, e_else);
                } else if (res == "let") {
                    auto id = readName();
                    auto temp = readName();

                    if (temp != "=")
                        break;

                    auto *e_value = recognizeExpression();

                    temp = readName();
                    if (temp != "in") {
                        delete e_value;
                        break;
                    }

                    auto *e_body = recognizeExpression();
                    exp = new Let(id, e_value, e_body);
                } else if (res == "function") {
                    auto id = readName();
                    auto f_body = recognizeExpression();

                    exp = new Function(id, f_body);
                } else if (res == "call") {
                    auto *f_exp = recognizeExpression();
                    auto *arg_expr = recognizeExpression();

                    exp = new Call(f_exp, arg_expr);
                } else if (res == "set") {
                    auto id = readName();
                    Expression *e_val = recognizeExpression();

                    exp = new Set(id, e_val);
                } else if (res == "block") {
                    std::vector<Expression *> exp_list;
                    Expression *t;
                    while ((t = recognizeExpression()) != nullptr)
                        exp_list.push_back(t);

                    exp = new Block(exp_list);
                }
            }
        }
        if (exp)
            current_char = (char) input.get();
        return exp;
    }
};