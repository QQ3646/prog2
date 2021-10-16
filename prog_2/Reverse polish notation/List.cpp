#include "List.h"

ListNode::ListNode(Expression *value, ListNode *previous) : value(value), next(nullptr), previous(previous) {}

List::List() : start(nullptr), end(nullptr), len(0) {}

Expression *List::pop() {
    if (start == end and start == nullptr)
        return nullptr;
    Expression *temp = end->value;
    if (start == end) {
        delete start;
        start = (end = nullptr);
    } else {
        ListNode *tempLN = end->previous;
        delete end;
        end = tempLN;
        end->next = nullptr;
    }
    len--;
    return temp;
}

void List::add_to_end(Expression *v) {
    if (start == nullptr and end == nullptr) {
        start = (end = new ListNode(v, nullptr));
    } else
        end = (end->next = new ListNode(v, end));
    len++;
}

void List::add_op_to_end(char op) {
    Expression *second = pop(), *first = pop();

    switch (op) {
        case '+':
            this->add_to_end(new Add(first, second));
            break;
        case '-':
            this->add_to_end(new Sub(first, second));
            break;
        case '*':
            this->add_to_end(new Mul(first, second));
            break;
        case '/':
            this->add_to_end(new Div(first, second));
            break;
    }
}


Expression *List::operator[](int n) {
    ListNode *current_pos = start;
    for (int i = 0; i < n and current_pos != nullptr; ++i) { current_pos = current_pos->next; }
    if (current_pos == nullptr)
        throw std::runtime_error("Invalid index of list!");
    return current_pos->value;
}

List::~List() {
    ListNode *temp = start->next;
    delete start;
    while (temp != nullptr) {
        delete temp->value;
        ListNode *ttemp = temp->next;
        delete temp;
        temp = ttemp;
    }
}
