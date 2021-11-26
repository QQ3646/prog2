#include "LexicalAnalyser.h"

int main() {
    std::ifstream ifstream("input.txt");
//    LexicalAnalyser analyser(ifstream);
    Expression *exp = LexicalAnalyser(ifstream).recognizeExpression();
//    exp->print();
    Environment env;
    Expression *new_e = exp->eval(env);
//    exp->print();
//    std::cout << std::endl;
    new_e->print();
}
