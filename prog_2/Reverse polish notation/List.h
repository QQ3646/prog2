#include "../Expressions/SubClasses.h"

class List;

class ListNode {
    Expression *value;
    ListNode *next, *previous;

    friend class List;
public:
    ListNode(Expression *value, ListNode *previous);

//    ~ListNode();
};

class List {
    ListNode *start, *end;

    int len;
public:
    List();

    int get_len() const;

    Expression *pop();

    void add_to_end(Expression *v);

    void add_op_to_end(char op);

    Expression *operator[](int n);

    ~List();
};



