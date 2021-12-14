#include "LexicalAnalyser.h"

class Interpreter {
    std::ifstream input;
    std::ofstream output;

    Expression* main = nullptr;
    Expression* eval_main = nullptr;
public:
    Interpreter(const char* inputFileName = "input.txt", const char* outputFileName = "output.txt");

    Interpreter& read();

    Interpreter& execute();

    void print();

    void print(std::ostream& os);

    ~Interpreter();
};