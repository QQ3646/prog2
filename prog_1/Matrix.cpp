#include "Matrix.h"
#include "iostream"

Matrix::Matrix() {
    this->size = 0;
    this->values = nullptr;
}

Matrix::Matrix(int size) : size(size) {
    this->values = new int*[size];
    for (int i = 0; i < size; i++) {
        this->values[i] = new int[size];
        for (int j = 0; j < size; ++j)
            this->values[i][j] = 0;
        this->values[i][i] = 1;
    }
}
// Мне не нравится эти одинаковые конструкторы, но что поделать
// Пытался в верхнем вызвать нижний конструктор, но это не сработало так как мне надо
Matrix::Matrix(int size, int* values) : size(size) {
    this->values = new int*[size];
    for (int i = 0; i < size; i++) {
        this->values[i] = new int[size];
        for (int j = 0; j < size; ++j)
            this->values[i][j] = 0;
        this->values[i][i] = values[i];
    }
}

Matrix::Matrix(int size, int** values) : size(size) {
    this->values = values;
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
            newValues[i][j] = this->values[i][j] + a.values[i][j];
    }
    return Matrix(size, newValues);
}

// Сравнение матриц
bool Matrix::operator==(const Matrix &b) {
    if (size != b.size)
        return false;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (this->values[i][j] != b.values[i][j])
                return false;
    return true;
}

// Транспонирование матрицы
Matrix Matrix::operator~() {
    int** newValues = new int*[size];
    for (int i = 0; i < size; ++i) {
        newValues[i] = new int[size];
        for (int j = 0; j < size; ++j)
            newValues[i][j] = this->values[j][i];
    }
    return Matrix(size, newValues);
}

// Вывод матрицы на экран
void Matrix::operator+() {
    for (int i = 0; i < this->size; ++i) {
        for (int j = 0; j < this->size; ++j) {
            printf("%d ",  this->values[i][j]);
        }
        printf("\n");
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < size; ++i)
        delete[] values[i];
    delete[] values;
}
