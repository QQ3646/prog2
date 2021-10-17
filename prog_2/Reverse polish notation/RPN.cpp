#include "RPN.h"

Node::Node(const String &string, Node *previous) : value(string), previous(previous), next(nullptr) {}


RPN::RPN() : startStack(nullptr), endStack(nullptr), endOperation(nullptr) {}

void RPN::add_to_stack(const String &string) {
    if (startStack == nullptr and endStack == nullptr)
        startStack = (endStack = new Node(string, endStack));
    else
        endStack = (endStack->next = new Node(string, endStack));
}

void RPN::add_to_op_Stack(char operation) {
    if (endOperation == nullptr)
        endOperation = new Node(String(operation), endOperation);
    else
        endOperation = (endOperation->next = new Node(String(operation), endOperation));
}

char RPN::pop_operation() {
    if (endOperation == nullptr)
        return '\0';
    Node *temp = endOperation->previous;
    char returnValue = endOperation->value[0];
    delete endOperation;
    endOperation = temp;
    return returnValue;
}

String RPN::pop_stack() {
    if (startStack == nullptr)
        return {'\0'};
    Node *temp = startStack->next;
    String a = startStack->value;
    delete startStack;
    startStack = temp;
    if (temp == nullptr)
        endStack = startStack;
    return a;
}

char RPN::get_last_op() {
    if (endOperation == nullptr)
        return '\0';
    return endOperation->value[0];
}

RPN::~RPN() {
    Node *current = endStack;
    while (current != nullptr) {
        Node *temp = endStack->previous;
        delete current;
        current = temp;
    }
    current = endOperation;
    while (current != nullptr) {
        Node *temp = endOperation->previous;
        delete current;
        current = temp;
    }
}
