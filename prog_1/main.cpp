#include <iostream>
#include "Matrix.h"
#include "cstdio"

std::ostream &operator<<(std::ostream &ostream, const Matrix &matrix) {
    for (int i = 0; i < matrix.size; ++i) {
        for (int j = 0; j < matrix.size; ++j)
            ostream << matrix.values[i][j] << " ";
        ostream << std::endl;
    }
    return ostream;
}

enum MatrixNames {
    A, B, C, D,
};

int main() {
    const int MATRIX_COUNT = 4;

    int outputMode;
    std::cin >> outputMode;

    if (outputMode == 1) {
        std::freopen("input.txt", "r", stdin);
        std::freopen("output.txt", "w", stdout);
    }

    int size, k;
    std::cin >> size >> k;

    int ***buffer = new int **[MATRIX_COUNT];
    Matrix *matrixArr = new Matrix[MATRIX_COUNT];
    for (int i = 0; i < MATRIX_COUNT; ++i) {
        buffer[i] = new int *[size];
        for (int j = 0; j < size; ++j) {
            buffer[i][j] = new int[size];
            for (int l = 0; l < size; ++l)
                std::cin >> buffer[i][j][l];
        }
        matrixArr[i] = Matrix(size, buffer[i]);
    }

    Matrix km(size, k);

    std::cout << (matrixArr[A] + matrixArr[B] * ~matrixArr[C] + km) * ~matrixArr[D];
}
