#include <iostream>
#include "Matrix.h"

int main() {
    Matrix a(2);
    int size = 2;
    int* d = new int[2];
    for (int i = 0; i < size; i++) {
            scanf("%d", &d[i]);
    }
    Matrix b(2, d);
    +b;
    +(a + b);
    int** mn = new int*[size];
    for (int i = 0; i < size; i++) {
        mn[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            scanf("%d", &mn[i][j]);
        }
    }
    Matrix another(2, mn);
    +another;
    if (another == (a + b))
        printf("yep");
    else
        printf("nope");
    +~another;
}
