#pragma once
#include "Matrix.h"

class Node {
    Node *next;
    MatrixPart *mp_adress;

    friend class List;
public:
    Node(MatrixPart *mp_adress);

    MatrixPart *get_adress();
};

class List {
    Node *head, *end;
public:
    List();

    void push_back(MatrixPart *mp_adress);

    void find_and_delete(MatrixPart *mp_adress);

    void make_inaccessible();

    ~List();
};

