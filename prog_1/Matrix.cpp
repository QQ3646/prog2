#include "Matrix.h"
#include "iostream"

Matrix::Matrix() {
    size = 0;
    values = nullptr;
    column = nullptr;
}

Matrix::Matrix(int size) : Matrix(size, 1) {}

Matrix::Matrix(int size, int value) : size(size) {
    values = new int*[size];
    for (int i = 0; i < size; i++) {
        values[i] = new int[size];
        for (int j = 0; j < size; ++j)
            values[i][j] = 0;
        values[i][i] = value;
    }
    column = nullptr;
}
// Мне не нравится эти одинаковые конструкторы, но что поделать
// Пытался в верхнем вызвать нижний конструктор, но это не сработало так как мне надо
Matrix::Matrix(int size, int* value) : size(size) {
    values = new int*[size];
    for (int i = 0; i < size; i++) {
        values[i] = new int[size];
        for (int j = 0; j < size; ++j)
            values[i][j] = 0;
        values[i][i] = value[i];
    }
    column = nullptr;
}

Matrix::Matrix(int size, int** value) : size(size) {
    values = value;
    column = nullptr;
}

// Получение минора
Matrix Matrix::getMinor(int a, int b) {
    int** newValues = new int*[size - 1];
    int ni = 0, nj = 0;
    for (int i = 0; i < size; ++i) {
        if (i == a - 1)
            continue;
        newValues[ni] = new int[size - 1];
        for (int j = 0; j < size; ++j) {
            if (j == b - 1)
                continue;
            newValues[ni][nj++] = values[i][j];
        }
        ni++;
        nj = 0;
    }
    return Matrix(size - 1, newValues);
}

// Сложение матриц
Matrix Matrix::operator+ (const Matrix& a) const {
    if (size != a.size) {
        std::cout << "Ошибка: размеры матриц не совпадают.";
        exit(1);
    }
    int** newValues = new int*[size];
    for (int i = 0; i < size; i++) {
        newValues[i] = new int[size];
        for (int j = 0; j < size; j++)
            newValues[i][j] = values[i][j] + a.values[i][j];
    }
    return Matrix(size, newValues);
}

// Сравнение матриц
bool Matrix::operator==(const Matrix &b) {
    if (size != b.size)
        return false;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (values[i][j] != b.values[i][j])
                return false;
    return true;
}

// Вывод матрицы на экран
void Matrix::operator+() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ",  values[i][j]);
        }
        printf("\n");
    }
}

// Умножение матриц
Matrix Matrix::operator*(const Matrix &b) const {
    int** newValues = new int*[size];
    for (int i = 0; i < size; i++) {
        newValues[i] = new int[size];
        for (int j = 0; j < size; j++) {
            int tempSum = 0;
            for (int k = 0; k < size; ++k) {
                tempSum += values[i][k] * b.values[k][j];
            }
            newValues[i][j] = tempSum;
        }
    }
    return Matrix(size, newValues);
}

// Взятие строки
int* Matrix::operator[](int num) {
    return values[num];
}

// Взятие столбца
int* Matrix::operator()(int num) {
    column = new int[size];
    for (int i = 0; i < size; ++i) {
        column[i] = values[i][num - 1];
    }
    return column;
}

// Транспонирование матрицы
Matrix Matrix::operator~() {
    int** newValues = new int*[size];
    for (int i = 0; i < size; ++i) {
        newValues[i] = new int[size];
        for (int j = 0; j < size; ++j)
            newValues[i][j] = values[j][i];
    }
    return Matrix(size, newValues);
}

Matrix::~Matrix() {
    for (int i = 0; i < size; ++i)
        delete[] values[i];
    delete[] values;
    delete[] column;
}
