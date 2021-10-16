#include "List.h"

class Node {
    Node *previous, *next;

    String value;

    friend class RPN;
public:
    Node(const String &string, Node *previous);

};

class RPN {
    Node *startStack;
    Node *endStack, *endOperation;
//    List *l;
public:
    RPN();

    void add_to_stack(const String &string);

    void add_to_op_Stack(char operation);

    char pop_operation();

    String pop_stack();

    char get_last_op();

    ~RPN();
};

