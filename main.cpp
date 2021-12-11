//#include "LexicalAnalyser.h"
#include "Interpreter.h"

int main() {
//    std::ifstream ifstream("input.txt");
//    Expression *exp = LexicalAnalyser(ifstream)();
//    Environment env;
//    Expression *new_e = exp->eval(env);
//    std::cout << new_e;
//    delete exp;
//    delete new_e;
    Interpreter inter("input.txt", "output.txt");
    inter.read().execute().print();
}
