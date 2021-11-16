#include <fstream>
#include "Environment.h"

class LexicalAnalyser {
    std::ifstream &input;
    char current_char;

    void skipSpaces() {
        while ((current_char = (char) input.get()) == ' ' || current_char == '\n' || current_char == '\t');
    }

    std::string readName() {
        std::string res{};
        skipSpaces();
        do
            res += current_char;
        while (isalpha((current_char = (char) input.get())));
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

    explicit LexicalAnalyser(std::ifstream &input) : input(input), current_char('\0') {
//        std::string temp;
//        int scope_deep = 0;
//        while (!input.eof()) {
//            input >> temp;
//            switch(temp) {
//                case "Val":
//                    break;
//                case "Var":
//                    break;
//                case "add":
//                    break;
//                case "if":
//                    break;
//                case "let":
//                    std::string id;
//                    input >> id;
//                    input >> temp;
//
//
//                    break;
//                case "function":
//                    break;
//                case "call":
//                    break;
//            }
//
//        }

    }

    Expression *recognizeExpression() {
        do  {
            if (current_char == '(') {
                auto res = readName();
                if (res == "val") {
                    int value = readValue();
                    auto *exp = new Val(value);
                    current_char = (char) input.get();

                    return exp;
                } else if (res == "var") {
                    auto id = readName();
                    auto *exp = new Var(id);
                    current_char = (char) input.get();

                    return exp;
                } else if (res == "add") {
                    auto *e1 = recognizeExpression();
                    auto *e2 = recognizeExpression();
                    auto *exp = new Add(e1, e2);
                    current_char = (char) input.get();

                    return exp;
                } else if (res == "if") {
                    auto *e1 = recognizeExpression();
                    auto *e2 = recognizeExpression();
                    auto temp = readName();
                    if (temp != "then")
                        break;
                    auto *e_then = recognizeExpression();
                    temp = readName();
                    if (temp != "else")
                        break;
                    auto *e_else = recognizeExpression();
                    auto *exp = new If(e1, e2, e_then, e_else);
                    current_char = (char) input.get();

                    return exp;
                } else if (res == "let") {
                    auto id = readName();
                    auto temp = readName();
                    if (temp != "=")
                        break;
                    auto *e_value = recognizeExpression();
                    temp = readName();
                    if (temp != "in")
                        break;
                    auto *e_body = recognizeExpression();
                    auto *exp = new Let(id, e_value, e_body);
                    current_char = (char) input.get();

                    return exp;
                } else if (res == "function") {
                    auto id = readName();
                    auto f_body = recognizeExpression();
                    auto *exp = new Function(id, f_body);
                    current_char = (char) input.get();

                    return exp;
                } else if (res == "call") {
                    auto *f_exp = recognizeExpression();
                    auto *arg_expr = recognizeExpression();
                    auto *exp = new Call(f_exp, arg_expr);
                    current_char = (char) input.get();

                    return exp;
                }
            }
        } while ((current_char = (char) input.get()) != ')');
        return nullptr;
    }
};