#include "Interpreter.h"

Interpreter::Interpreter(const char* inputFileName, const char* outputFileName) : input(inputFileName),
                                                                                  output(outputFileName) {}

Interpreter& Interpreter::read() {
    main = LexicalAnalyser(input)();
    return *this;
}

Interpreter& Interpreter::execute() {
    try {
        //        if (!main->syntaxCheck())
        //            throw std::runtime_error("Invalid syntax!");
        Environment new_e;
        eval_main = main->eval(new_e);
    }
    catch (...) {
        output << "ERROR";
    }
    return *this;
}

void Interpreter::print() {
    print(output);
}

void Interpreter::print(std::ostream& os) {
    os << eval_main;
}

Interpreter::~Interpreter() {
    delete main;
    delete eval_main;

    input.close();
    output.close();
}