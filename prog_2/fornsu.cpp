#include "iostream"
#include "ctype.h"
#include <fstream>

class String {
    char *string;
    int size;

    int BUFFER_SIZE, ALLOCATED_SIZE;

    friend std::ostream &operator<<(std::ostream &ostream, const String &string);

    friend std::istream &operator>>(std::istream &istream, String &string);

    int get_len_of_NP(const char *string) {
        int i = 0;
        for (; string[i]; ++i);
        return i;
    }

public:

    String() : size(0), BUFFER_SIZE(2), ALLOCATED_SIZE(1) {
        string = new char[1];
        string[0] = '\0';
    }

    String(char a) {
        size = 1;
        string = new char[2];
        string[0] = a;
        string[1] = '\0';
        ALLOCATED_SIZE = 2;
        BUFFER_SIZE = 2;
    }

    String(const char *string) : String() {
        *this = string;
    }

    String(const String &string) : String(string.string) {}

    friend std::istream &operator>>(std::ifstream &istream, String &string) {
        delete[] string.string;

        string.string = new char[2];
        string.string[0] = '\0';
        string.ALLOCATED_SIZE = 2;
        string.size = 0;
        string.BUFFER_SIZE = 4;

        char temp = (char) istream.get();
        int current_index = 0;
        while (temp != -1) {
            if (current_index + 1 >= string.ALLOCATED_SIZE) {
                char *tempstr = new char[string.size + 1];
                int i = 0;
                for (; i < string.size; ++i)
                    tempstr[i] = string.string[i];
                tempstr[i] = '\0';

                delete[] string.string;
                string.string = new char[string.BUFFER_SIZE];
                string.ALLOCATED_SIZE = string.BUFFER_SIZE;

                for (i = 0; tempstr[i]; ++i)
                    string.string[i] = tempstr[i];
                string.string[i] = '\0';

                delete[] tempstr;

                string.BUFFER_SIZE *= 2;
            }
            string.string[current_index++] = temp;
            string.string[current_index] = '\0';
            string.size++;
            temp = (char) istream.get();
        }
        return istream;
    }

    int get_len() const { return size; }

    String &operator=(const char *string) {
        size = get_len_of_NP(string);
        delete[] this->string;
        this->string = new char[size + 1];
        for (int i = 0; i < size; ++i) { this->string[i] = string[i]; }
        this->string[size] = '\0';
        ALLOCATED_SIZE = size + 1;
        BUFFER_SIZE = ALLOCATED_SIZE * 2;
        return *this;
    }

    String &operator=(const String &string) {
        if (this == &string)
            return *this;
        *this = string.string;  // Здесь и в других местах, я вызываю методы от поля string, тем самым теряя выгоду от того, что размер уже посчитан
        // Вроде бы и убираю копипасту, но тем не менее любая оперция, даже с уже известым размером становится O(n)
        // Мне кажется это слишком спорный момент, вроде и кода меньше, но зато время сильно возрастает впустую
        return *this;
    }

    String operator+(const char *string) {
        int newSize = size + get_len_of_NP(string);
        char *newStr = new char[newSize + 1];
        for (int i = 0; i < size; ++i)
            newStr[i] = this->string[i];
        for (int i = 0; string[i]; ++i)
            newStr[size + i] = string[i];
        newStr[newSize] = '\0';
        return {newStr};
    }

    String operator+(const String &string) {
        return *this + string.string;
    }

    bool operator==(const char *string) {
        if (size != get_len_of_NP(string))
            return false;
        for (int i = 0; i < size; ++i) {
            if (this->string[i] != string[i])
                return false;
        }
        return true;
    }

    bool operator==(const String &string) { return *this == string.string; }

    friend std::ostream &operator<<(std::ostream &ostream, const String &string) {
        for (int i = 0; i < string.size; ++i)
            ostream.put(string.string[i]);
        return ostream;
    }

    char operator[](int n) const {
        if (n > size)
            throw "Invalid index of string";
        return string[n];
    }

    String operator()(int a, int b) {
        if (a >= size || a > b)
            throw "Incorrect [a; b] indexes";
        int newSize = b - a;
        char *temp = new char[newSize + 1];
        for (int i = a; i < b; ++i) {
            temp[i - a] = string[i];
        }
        temp[newSize] = '\0';
        String str(temp);
        delete[] temp;
        return str;
    }

    ~String() {
        delete[] string;
    }
};

class Expression {
//    int type;
public:
    friend std::istream &operator>>(std::istream &istream, Expression &expression);

    virtual Expression *get_copy() = 0;

    virtual void print(std::ofstream &stream) = 0;

    virtual Expression *derivative(const String &string) = 0;

    virtual int eval(const String &string) = 0;

    virtual Expression *simplify() = 0;

    virtual ~Expression() {}
};

class Operations : public Expression {
protected:
    Expression *first, *second;
    char symbol;


    Operations(Expression *first, Expression *second, char symbol) {
        this->first = first;
        this->second = second;
        this->symbol = symbol;
    }

    void print(std::ofstream &stream) {
        stream << '(';
        first->print(stream);
        stream << symbol;
        second->print(stream);
        stream << ')';
    };

    ~Operations() {
        delete first;
        delete second;
    }
};

class Number : public Expression {
    int value;

public:
    Number(int value) {
        this->value = value;
    }

    Expression *get_copy() {
        return new Number(value);
    }

    Expression *derivative(const String &a) {
        return new Number(0);
    }

    void print(std::ofstream &stream) {
        stream << value;
    }

    int eval(const String &a) { return value; }

    int get_value() const { return value; }

    Expression *simplify() { return this; }
};

class Variable : public Expression {
    String name;
public:
    Variable(const String &string) {
        this->name = string;
    }

    Expression *get_copy() {
        return new Variable(name);
    }

    void print(std::ofstream &stream) {
        stream << name;
    }

    Expression *derivative(const String &string) {
        for (int i = 0; string[i] != '\0' && this->name[i] != '\0'; ++i) {
            if (this->name[i] != string[i])
                return new Number(0);
        }
        return new Number(1);
    }

    int eval(const String &string) { return 0; }

    Expression *simplify() { return nullptr; }

    const String &get_name() { return name; }
};

class Add : public Operations {
public:
    Add(Expression *first, Expression *second) : Operations(first, second, '+') {}

    Expression *get_copy() {
        return new Add(first->get_copy(), second->get_copy());
    }

    Expression *derivative(const String &string) {
        return new Add(first->derivative(string), second->derivative(string));
    }

    int eval(const String &string) { return first->eval(string) + second->eval(string); }

    Expression *simplify() { return nullptr; }
};

class Sub : public Operations {
public:
    Sub(Expression *first, Expression *second) : Operations(first, second, '-') {}

    Expression *get_copy() {
        return new Sub(first->get_copy(), second->get_copy());
    }

    Expression *derivative(const String &string) {
        return new Sub(first->derivative(string), second->derivative(string));
    }

    int eval(const String &string) { return first->eval(string) - second->eval(string); }

    Expression *simplify() { return nullptr; }
};

class Mul : public Operations {
public:
    Mul(Expression *first, Expression *second) : Operations(first, second, '*') {}

    Expression *get_copy() {
        return new Mul(first->get_copy(), second->get_copy());
    }

    Expression *derivative(const String &string) {
        return new Add(new Mul(first->derivative(string), second->get_copy()),
                       new Mul(first->get_copy(), second->derivative(string)));
    }

    int eval(const String &string) { return first->eval(string) * second->eval(string); }

    Expression *simplify() {
        return nullptr;
    }
};

class Div : public Operations {
public:
    Div(Expression *first, Expression *second) : Operations(first, second, '/') {}

    Expression *get_copy() {
        return new Div(first->get_copy(), second->get_copy());
    }

    Expression *derivative(const String &string) {
        return new Div(new Sub(new Mul(first->derivative(string), second->get_copy()), new Mul(first->get_copy(), second->derivative(string))),
                       new Mul(second->get_copy(), second->get_copy()));
    }

    int eval(const String &string) { return first->eval(string) / second->eval(string); }

    Expression *simplify() {
        return nullptr;
    }
};


class List;

class ListNode {
    Expression *value;
    ListNode *next, *previous;

    friend class List;

public:
    ListNode(Expression *value, ListNode *previous) : value(value), next(nullptr), previous(previous) {}

};

class List {
    ListNode *start, *end;

    int len;
public:
    List() : start(nullptr), end(nullptr), len(0) {}

    int get_len() const { return len; }

    Expression *pop() {
        if (start == end && start == nullptr)
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

    void add_to_end(Expression *v) {
        if (start == nullptr && end == nullptr) {
            start = (end = new ListNode(v, nullptr));
        } else
            end = (end->next = new ListNode(v, end));
        len++;
    }

    void add_op_to_end(char op) {
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


    Expression *operator[](int n) {
        ListNode *current_pos = start;
        for (int i = 0; i < n && current_pos != nullptr; ++i) { current_pos = current_pos->next; }
        if (current_pos == nullptr)
            throw "Invalid index of list!";
        return current_pos->value;
    }


    ~List() {
//    bool b = true;
        ListNode *temp = start->next;
        delete start;
        while (temp != nullptr) {
            delete temp->value;
            ListNode *ttemp = temp->next;
            delete temp;
            temp = ttemp;
        }
    }

};

class Node {
    Node *previous, *next;

    String value;

    friend class RPN;

public:
    Node(const String &string, Node *previous) : value(string), previous(previous), next(nullptr) {}

};

class RPN {
    Node *startStack;
    Node *endStack, *endOperation;
//    List *l;
public:
    RPN() : startStack(nullptr), endStack(nullptr), endOperation(nullptr) {}

    void add_to_stack(const String &string) {
        if (startStack == nullptr && endStack == nullptr)
            startStack = (endStack = new Node(string, endStack));
        else
            endStack = (endStack->next = new Node(string, endStack));
    }

    void add_to_op_Stack(char operation) {
        if (endOperation == nullptr)
            endOperation = new Node(String(operation), endOperation);
        else
            endOperation = (endOperation->next = new Node(String(operation), endOperation));
    }

    char pop_operation() {
        if (endOperation == nullptr)
            return '\0';
        Node *temp = endOperation->previous;
        char returnValue = endOperation->value[0];
        delete endOperation;
        endOperation = temp;
//    if (temp != nullptr)
//        temp->next = nullptr;
        return returnValue;
    }

    String pop_stack() {
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

    char get_last_op() {
        if (endOperation == nullptr)
            return '\0';
        return endOperation->value[0];
    }

    ~RPN() {
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
};


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
    Expression *e = c->derivative("x");
    e->print(out);
    delete c;
    delete e;
    out.close();
    in.close();
}
