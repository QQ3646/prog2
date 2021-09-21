#include <iostream>
#include "Matrix.h"

enum namesOfMatrix {
    A, B, C, D,
};

int main() {
    int size;
    scanf("%d", &size);
    int k;
    scanf("%d", &k);
    int*** buffer = new int**[4];
    for (int i = 0; i < 4; ++i) {
        buffer[i] = new int*[size];
        for (int j = 0; j < size; ++j)
            buffer[i][j] = new int[size];
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int l = 0; l < size; ++l) {
                scanf("%d", &buffer[i][j][l]);
            }
        }
    }
    // Пытался через массив матриц, но выдавало сигсег, так что лучше так
    Matrix a = Matrix(size, buffer[0]);
    a[1][1] = 5;
    for (int i = 0; i < size; ++i) {
        printf("%d ", a[1][i]);
    }
}
