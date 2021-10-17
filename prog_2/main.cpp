#include <iostream>
#include "Expressions/SubClasses.h"
#include "Reverse polish notation/RPN.h"

Expression *parse(std::ifstream &istream) {
    RPN rpn;
    String string;
    istream >> string;
    for (int i = 0; i < string.get_len(); ++i) {
        if (isalnum(string[i])) {
            int j = 0;
            for (; isalnum(string[i + j]); ++j) {}
            rpn.add_to_stack(string(i, i + j));
            i += j - 1;
            continue;
        } else {
            if (string[i] == '(') rpn.add_to_op_Stack('(');
            else if (string[i] == '+' || string[i] == '-') {
                if (rpn.get_last_op() == '-' || rpn.get_last_op() == '+' ||
                    rpn.get_last_op() == '/' || rpn.get_last_op() == '*')
                    rpn.add_to_stack(String(rpn.pop_operation()));
                rpn.add_to_op_Stack(string[i]);
            } else if (string[i] == '*' || string[i] == '/') {
                if (rpn.get_last_op() == '*' || rpn.get_last_op() == '/' )
                    rpn.add_to_stack(String(rpn.pop_operation()));
                rpn.add_to_op_Stack(string[i]);
            } else {
                while (rpn.get_last_op() != '(') {
                    rpn.add_to_stack(String(rpn.pop_operation()));
                }
                rpn.pop_operation();
            }
        }
    }
    char temp = rpn.pop_operation();
    while (temp != '\0') {
        rpn.add_to_stack(String(temp));
        temp = rpn.pop_operation();
    }
    String current;
    List list;
    while ((current = rpn.pop_stack())[0] != '\0') {
        if (isdigit(current[0])) {
            int num = 0;
            for (int i = 0; current[i]; ++i) { num = num * 10 + (current[i] - '0'); }
            list.add_to_end(new Number(num));
        } else if (isalpha(current[0])) {
            list.add_to_end(new Variable(current));
        } else {
            list.add_op_to_end(current[0]);
        }
    }
    Expression *e = list[0];

    return e;
}

int main() {
    std::ofstream out;
    std::ifstream in;
    out.open("output.txt");
    in.open("input.txt");

    Expression *c = parse(in);
//    Expression *e = c->derivative("x");
//    e->print(out);
    c->simple()->print(out);
    delete c;
//    delete e;
    out.close();
    in.close();
}
