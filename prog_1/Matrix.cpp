#include "Matrix.h"
#include "iostream"

MatrixPart::MatrixPart(int num, Matrix &matrix, bool flag) : num(num), matrix(matrix), flag(flag), matrixExist(true) {
    if (num >= matrix.size)  // Вынес сюда проверку на недействительный номер чтобы не писать дважды
        throw "Invalid number!";
    matrix.usedPart.push_back(this);
}

int &MatrixPart::operator[](int n) {
    if (!matrixExist)
        throw "Matrix doesn't exist!";
    if (n >= matrix.size)
        throw "Invalid number!";
    if (flag)
        return matrix.values[n][num];
    else
        return matrix.values[num][n];
}

void MatrixPart::makeInaccessible() {
    matrixExist = false;
}

void Matrix::allocateMem() {
    values = new int *[size];
    for (int i = 0; i < size; i++) {
        values[i] = new int[size];
        for (int j = 0; j < size; ++j)
            values[i][j] = 0;
    }
}

void Matrix::deleteMem() {
    for (int i = 0; i < size; ++i)
        delete[] values[i];
    delete[] values;
}

Matrix::Matrix() {
    usedPart = std::vector<MatrixPart*>();
    size = 0;
    values = nullptr;
}

Matrix::Matrix(int size) : Matrix(size, 1) {}

// Вынес все аллокации памяти в один конструктор, не знаю, надо было ли, но мне так нравится чуть больше
Matrix::Matrix(int size, int value) : size(size) {
    usedPart = std::vector<MatrixPart*>();
    allocateMem();
    for (int i = 0; i < size; ++i)
        values[i][i] = value;
}

Matrix::Matrix(int size, int *values) : Matrix(size, 0) {
    for (int i = 0; i < size; ++i)
        this->values[i][i] = values[i];
}

Matrix::Matrix(int size, std::istream &istream) : Matrix(size, 0) {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            istream >> values[i][j];
}

Matrix &Matrix::operator=(const Matrix &matrix) {
    if (this == &matrix)
        return *this;

    for (MatrixPart* i : usedPart)
        i->makeInaccessible();
    usedPart = matrix.usedPart;

    if (size != matrix.size) {
        int **newValues = new int *[matrix.size];
        for (int i = 0; i < matrix.size; ++i) {
            newValues[i] = new int[matrix.size];
            for (int j = 0; j < matrix.size; ++j)
                newValues[i][j] = matrix.values[i][j];
        }

        deleteMem();

        size = matrix.size;
        values = newValues;
    } else {
        for (int i = 0; i < matrix.size; ++i)
            for (int j = 0; j < matrix.size; ++j)
                values[i][j] = matrix.values[i][j];
    }
    return *this;
}

// Получение минора
Matrix Matrix::operator()(int a, int b) {
    if (a >= size or b >= size)
        throw "Invalid x or y";

    Matrix *newM = new Matrix(size - 1, 0);
    newM->values = new int*[size - 1];

    int ni = 0, nj = 0;
    for (int i = 0; i < size; ++i) {
        if (i == a - 1)
            continue;
        newM->values[ni] = new int[size - 1];
        for (int j = 0; j < size; ++j) {
            if (j == b - 1)
                continue;
            newM->values[ni][nj++] = values[i][j];
        }
        ni++;
        nj = 0;
    }
    return *newM;
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

// Сложение матриц
Matrix Matrix::operator+(const Matrix &a) const {
    if (size != a.size)
        throw "Size of the matrices does not match!";

    Matrix *newM = new Matrix(size, 0);
    newM->values = new int *[size];
    for (int i = 0; i < size; i++) {
        newM->values[i] = new int[size];
        for (int j = 0; j < size; j++)
            newM->values[i][j] = values[i][j] + a.values[i][j];
    }

    return *newM;
}

// Умножение матриц *надо допилить еще умножение на столбец, пожалуй
Matrix Matrix::operator*(const Matrix &b) const {
    if (size != b.size)
        throw "Size of the matrices does not match!";

    Matrix *newM = new Matrix(size, 0);
    newM->values = new int *[size];
    for (int i = 0; i < size; i++) {
        newM->values[i] = new int[size];
        for (int j = 0; j < size; j++) {
            int tempSum = 0;
            for (int k = 0; k < size; ++k)
                tempSum += values[i][k] * b.values[k][j];
            newM->values[i][j] = tempSum;
        }
    }
    return *newM;
}

// Взятие строки
MatrixPart Matrix::operator[](int num) { return {num, *this, false}; }

// Взятие столбца
MatrixPart Matrix::operator()(int num) { return {num, *this, true}; }

// Транспонирование матрицы
Matrix Matrix::operator~() {
    Matrix *newM = new Matrix(size, 0);
    newM->values = new int *[size];
    for (int i = 0; i < size; ++i) {
        newM->values[i] = new int[size];
        for (int j = 0; j < size; ++j)
            newM->values[i][j] = values[j][i];
    }
    return *newM;
}

std::ostream &operator<<(std::ostream &ostream, const Matrix &matrix) {
    for (int i = 0; i < matrix.size; ++i) {
        for (int j = 0; j < matrix.size; ++j)
            ostream << matrix.values[i][j] << " ";
        ostream << std::endl;
    }
    return ostream;
}

Matrix::~Matrix() {
    for (MatrixPart* i : usedPart)
        i->makeInaccessible();
    deleteMem();
}