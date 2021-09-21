#include "iostream"

class Matrix;

class MatrixColumn {
    int **pos;
public:
    MatrixColumn(int **pos);

    int& operator[](int num);

    ~MatrixColumn();
};


class Matrix {
    int size;
    int **values;
    friend std::ostream& operator<<(std::ostream &ostream, const Matrix &matrix);  // Пригодилось!
public:
    Matrix();

    Matrix(int size);

    Matrix(int size, int value);

    Matrix(int size, int *values);

    Matrix(int size, int **values);

    Matrix& operator=(const Matrix &matrix);

    Matrix getMinor(int a, int b);

    bool operator==(const Matrix &b);

    Matrix operator+(const Matrix &a) const;

    Matrix operator*(const Matrix &b) const;

    int *operator[](int num);

    MatrixColumn operator()(int num);

    Matrix operator~();

    ~Matrix();
};


