#include <iostream>
#include "Array.h"

class Matrix;

class MatrixPart {
    int num;
    const Matrix &matrix;
    bool flag;  // 0 - line
    // 1 - column
    bool matrixExist;

    friend class Matrix;

    friend class List<MatrixPart>;

    void makeInaccessible();

public:
    MatrixPart(int num, Matrix &matrix, bool flag);

    int &operator[](int num);

    ~MatrixPart();
};


class Matrix {
    int size;
    int **values;
    List<MatrixPart> *usedPart;

    friend std::istream &operator>>(std::istream &istream, Matrix &matrix);

    friend std::ostream &operator<<(std::ostream &ostream, const Matrix &matrix);  // Пригодилось!
    friend MatrixPart;

    void allocateMem();

    void deleteMem();

public:
    Matrix();

    Matrix(int size);

    Matrix(int size, int value);

    Matrix(int size, int *values);

    Matrix(int size, std::istream &istream);

    Matrix(const Matrix &matrix);

    Matrix &operator=(const Matrix &matrix);

    Matrix operator()(int a, int b);

    bool operator==(const Matrix &b);

    Matrix operator+(const Matrix &a) const;

    Matrix operator*(const Matrix &b) const;

    MatrixPart operator[](int num);

    MatrixPart operator()(int num);

    Matrix operator~();

    ~Matrix();

    operator size_t() const {
        std::string temp{};
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                temp += std::to_string(values[i][j]);
            }
        }
        std::hash<std::string> temphash;
        return temphash(temp);
    }
};
namespace std {
    template<>
    struct hash<Matrix> {
        size_t operator()(const Matrix &matrix) const noexcept{
            return matrix;
        }
    };
}