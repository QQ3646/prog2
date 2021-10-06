#include "iostream"
#include <vector>

class Matrix;

class MatrixPart {
    int num;
    const Matrix &matrix;
    bool flag;  // 0 - line
                // 1 - column
    bool matrixExist;
    friend Matrix;

    void makeInaccessible();
public:
    MatrixPart(int num, Matrix &matrix, bool flag);

    int& operator[](int num);
};


class Matrix {
    int size;
    int **values;
    std::vector<MatrixPart*> usedPart;

    friend std::ostream& operator<<(std::ostream &ostream, const Matrix &matrix);  // Пригодилось!
    friend MatrixPart;

    void allocateMem();

    void deleteMem();
public:
    Matrix();

    Matrix(int size);

    Matrix(int size, int value);

    Matrix(int size, int *values);

    Matrix(int size, std::istream &istream);

    Matrix& operator=(const Matrix &matrix);

    Matrix operator()(int a, int b);

    bool operator==(const Matrix &b);

    Matrix operator+(const Matrix &a) const;

    Matrix operator*(const Matrix &b) const;

    MatrixPart operator[](int num);

    MatrixPart operator()(int num);

    Matrix operator~();

    //std::ostream &operator<<(std::ostream &ostreamm, const Matrix &matrix);

    ~Matrix();
};


