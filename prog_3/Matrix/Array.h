template<typename V>
class List {
    class Node {
        Node *next;
        V *value;

        friend class List;

    public:
        Node(V *value) : next(nullptr), value(value) {};
    };

    Node *head, *endN;
public:
    void push_back(V *value) {
        Node *newN = new Node(value);
        if (head == nullptr && endN == nullptr) {
            head = newN;
            endN = newN;
        } else {
            endN->next = newN;
        }
    }

    void find_and_delete(V *value) {
        if (value == head->value && value == endN->value) {
            delete head;
            endN = head = nullptr;
        } else if (value == head->value) {
            Node *temp = head->next;
            delete head;
            head = temp;
        } else {
            Node *previousNode = nullptr;
            Node *currentNode = head;
            while (currentNode != nullptr and currentNode->value != value) {
                previousNode = currentNode;
                currentNode = currentNode->next;
            }
            if (currentNode == nullptr)
                return;
            Node *temp = currentNode->next;
            delete currentNode;
            previousNode->next = temp;
        }
    }

    void make_inaccessible() {
        Node *current = head;
        while (current != nullptr) {
            current->value->makeInaccessible();
            current = current->next;
        }
    }

    ~List() {
        Node *current = head;
        while (current != nullptr) {
            Node *temp = head->next;
            delete current;
            current = temp;
        }
    }

    class Iterator {
        Node *value;
        int pos;
    public:
        Iterator() : value(nullptr), pos(0) {}

        Iterator(Node *value, int pos) : value(value), pos(pos) {}

        Iterator(const Iterator &iterator) {
            *this = iterator;
        }

        V &operator*() {
            if (value != nullptr)
                return *value->value;
        }

        V *operator->(){
            if (value != nullptr)
                return value->value;
        }

        Iterator &operator=(const Iterator &iterator) {
            if (this == &iterator)
                return *this;
            *this = Iterator(iterator.value, iterator.pos);
            return *this;
        }

        bool operator==(const Iterator &iterator) { return value == iterator.value; }

        bool operator!=(const Iterator &iterator) { return !(*this == iterator); }

        Iterator operator++() {
            if (value == nullptr || value->next == nullptr)
                return Iterator();
            pos++;
            value = value->next;
            return *this;
        }
    };

    List<V>::Iterator begin() {
        return List<V>::Iterator(head, 0);
    }

    List<V>::Iterator end() {
        return List<V>::Iterator();
    }
};

