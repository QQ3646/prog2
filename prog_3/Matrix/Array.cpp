#include "Array.h"


Node::Node(MatrixPart *mp_adress) {
    next = nullptr;
    this->mp_adress = mp_adress;
}

List::List() {
    head = nullptr;
    end = nullptr;
}

//List::List(const List &list) {
//
//}

void List::push_back(MatrixPart *mp_adress) {
    Node *newN = new Node(mp_adress);
    if (head == nullptr && end == nullptr) {
        head = newN;
        end = newN;
    } else {
        end->next = newN;
    }
}

void List::find_and_delete(MatrixPart *mp_adress) {
    if (mp_adress == head->mp_adress) {
        Node *temp = head->next;
        delete head;
        head = temp;
    } else {
        Node *previousNode = nullptr;
        Node *currentNode = head;
        while (currentNode != nullptr and currentNode->mp_adress != mp_adress) {
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

void List::make_inaccessible() {
    Node *current = head;
    while (current != nullptr) {
        current->mp_adress->makeInaccessible();
        current = current->next;
    }
}

List::~List() {
    Node *current = head;
    while (current != nullptr) {
        Node *temp = head->next;
        delete current;
        current = temp;
    }
}