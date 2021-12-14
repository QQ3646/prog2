#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include "Expressions/SubExpressions.h"


class LexicalAnalyser {

    std::stringstream input_s;
public:
    explicit LexicalAnalyser(std::ifstream& input_stream) {
        std::string input_str(std::istreambuf_iterator<char>(input_stream), {});
        input_str = regex_replace(input_str, std::regex(R"((\s*\(\s*)|(\s*\)\s*))"), " $& "); // добавить пробелы
        input_str = regex_replace(input_str, std::regex(R"(\s+)"), " "); // убрать лишние пробельные символы
        input_s << input_str;
    }

    Expression* operator()() {
        std::string t;
        Expression* retExpr = nullptr;
        while (!input_s.eof() && !retExpr && t != ")") {
            input_s >> t;
            if (t == "(") {
                input_s >> t;
                if (t == "val") {
                    int value;
                    input_s >> value;

                    retExpr = new Val(value);
                }
                else if (t == "var") {
                    std::string id;
                    input_s >> id;

                    retExpr = new Var(id);
                }
                else if (t == "add") {
                    Expression *e_l = operator()(), *e_r = operator()();
                    retExpr = new Add(e_l, e_r);
                }
                else if (t == "if") {
                    Expression* leftEx = operator()(), * rightEx = operator()();

                    std::string temp;
                    input_s >> temp;
                    if (temp != "then")
                        std::cout << "Ага, опять, негодник, ключевые слова не вставляешь, да?";

                    Expression* thenEx = operator()();

                    input_s >> temp;
                    if (temp != "else")
                        std::cout << "Ага, опять, негодник, ключевые слова не вставляешь, да?";

                    Expression* elseEx = operator()();

                    retExpr = new If(leftEx, rightEx, thenEx, elseEx);
                }
                else if (t == "let") {
                    std::string id;
                    input_s >> id;

                    std::string temp;
                    input_s >> temp;
                    if (temp != "=")
                        std::cout << "Ага, опять, негодник, ключевые слова не вставляешь, да?";

                    Expression* valueEx = operator()();

                    input_s >> temp;
                    if (temp != "in")
                        std::cout << "Ага, опять, негодник, ключевые слова не вставляешь, да?";

                    Expression* bodyEx = operator()();

                    retExpr = new Let(id, valueEx, bodyEx);
                }
                else if (t == "function") {
                    std::string id;
                    input_s >> id;

                    Expression *b = operator()();
                    retExpr = new Function(id, b);
                }
                else if (t == "call") {
                    auto* f_e = operator()();
                    auto* arg_e = operator()();
                    retExpr = new Call(f_e, arg_e);
                }
                else if (t == "block") {
                    std::vector<Expression*> t_v;
                    Expression* t_e = operator()();
                    while (t_e != nullptr) {
                        t_v.push_back(t_e);
                        t_e = operator()();
                    }

                    retExpr = new Block(t_v);
                }
                else if (t == "set") {
                    std::string id;
                    input_s >> id;

                    auto* e = operator()();

                    retExpr = new Set(id, e);
                }
            }
        }
        if (retExpr)
            input_s >> t;
        return retExpr;
    }
};