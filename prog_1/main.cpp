#include <iostream>
#include "Matrix.h"
#include "cstdio"



enum MatrixNames {
    A, B, C, D,
};

int main() {
    const int MATRIX_COUNT = 4;

    int outputMode = 1;
//    std::cin >> outputMode;

    if (outputMode == 1) {
        std::freopen("input.txt", "r", stdin);
        std::freopen("output.txt", "w", stdout);
    }

    int size, k;
    std::cin >> size >> k;

    Matrix km(size, k);
    Matrix *matrixArr = new Matrix[MATRIX_COUNT];
    for (int i = 0; i < MATRIX_COUNT; ++i) {
        matrixArr[i] = Matrix(size, std::cin);
    }

    std::cout << (matrixArr[A] + matrixArr[B] * ~matrixArr[C] + km) * ~matrixArr[D];
    delete[] matrixArr;
//    Matrix a(size, std::cin);
//    std::cout << a;
//    a[1][1] = 1000;
////    std::cout << a;
//    a(2)[1] = -1000;
//    std::cout << a;
//    std::cout << a(1, 1);

}
