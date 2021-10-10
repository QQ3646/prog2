#include <iostream>
#include "Array.h"
#include "cstdio"

#pragma once
#include "iostream"
#include "Array.h"

class Matrix;
class List;

class MatrixPart {
    int num;
    const Matrix &matrix;
    bool flag;  // 0 - line
                // 1 - column
    bool matrixExist;
    friend class Matrix;
    friend class List;

    void makeInaccessible();
public:
    MatrixPart(int num, Matrix &matrix, bool flag);

    int& operator[](int num);

    ~MatrixPart();
};


class Matrix {
    int size;
    int **values;
    List *usedPart;

    friend std::ostream& operator<<(std::ostream &ostream, const Matrix &matrix);  // Пригодилось!
    friend MatrixPart;

    void allocateMem();

    void deleteMem();
public:
   MatrixPart::MatrixPart(int num, Matrix &matrix, bool flag) : num(num), matrix(matrix), flag(flag), matrixExist(true) {
       if (num >= matrix.size)  // Вынес сюда проверку на недействительный номер чтобы не писать дважды
           throw "Invalid number!";
       matrix.usedPart->push_back(this);
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
   
   MatrixPart::~MatrixPart() {
       if (matrixExist)
           matrix.usedPart->find_and_delete(this);
   }
   
   void allocateMem() {
       values = new int *[size];
       for (int i = 0; i < size; i++) {
           values[i] = new int[size];
           for (int j = 0; j < size; ++j)
               values[i][j] = 0;
       }
   }
   
   void deleteMem() {
       for (int i = 0; i < size; ++i)
           delete[] values[i];
       delete[] values;
   }
   
   Matrix() {
       usedPart = new List();
       size = 0;
       values = nullptr;
   }
   
   Matrix(int size) : Matrix(size, 1) {}
   
   // Вынес все аллокации памяти в один конструктор, не знаю, надо было ли, но мне так нравится чуть больше
   Matrix(int size, int value) : size(size) {
       usedPart = new List();
       allocateMem();
       for (int i = 0; i < size; ++i)
           values[i][i] = value;
   }
   
   Matrix(int size, int *values) : Matrix(size, 0) {
       for (int i = 0; i < size; ++i)
           this->values[i][i] = values[i];
   }
   
   Matrix(int size, std::istream &istream) : Matrix(size, 0) {
       for (int i = 0; i < size; ++i)
           for (int j = 0; j < size; ++j)
               istream >> values[i][j];
   }
   
   
   
   Matrix &operator=(const Matrix &matrix) {
       if (this == &matrix)
           return *this;
   
       usedPart->make_inaccessible();
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
   Matrix operator()(int a, int b) {
       if (a >= size or b >= size)
           throw "Invalid x or y";
   
       Matrix newM(size - 1, 0);
       newM.values = new int*[size - 1];
   
       int ni = 0, nj = 0;
       for (int i = 0; i < size; ++i) {
           if (i == a - 1)
               continue;
           newM.values[ni] = new int[size - 1];
           for (int j = 0; j < size; ++j) {
               if (j == b - 1)
                   continue;
               newM.values[ni][nj++] = values[i][j];
           }
           ni++;
           nj = 0;
       }
       return newM;
   }
   
   // Сравнение матриц
   bool operator==(const Matrix &b) {
       if (size != b.size)
           return false;
       for (int i = 0; i < size; i++)
           for (int j = 0; j < size; j++)
               if (values[i][j] != b.values[i][j])
                   return false;
       return true;
   }
   
   // Сложение матриц
   Matrix operator+(const Matrix &a) const {
       if (size != a.size)
           throw "Size of the matrices does not match!";
   
       Matrix newM(size, 0);
       newM.values = new int *[size];
       for (int i = 0; i < size; i++) {
           newM.values[i] = new int[size];
           for (int j = 0; j < size; j++)
               newM.values[i][j] = values[i][j] + a.values[i][j];
       }
   
       return newM;
   }
   
   // Умножение матриц
   Matrix operator*(const Matrix &b) const {
       if (size != b.size)
           throw "Size of the matrices does not match!";
   
       Matrix newM(size, 0);
       newM.values = new int *[size];
       for (int i = 0; i < size; i++) {
           newM.values[i] = new int[size];
           for (int j = 0; j < size; j++) {
               int tempSum = 0;
               for (int k = 0; k < size; ++k)
                   tempSum += values[i][k] * b.values[k][j];
               newM.values[i][j] = tempSum;
           }
       }
       return newM;
   }
   
   // Взятие строки
   MatrixPart operator[](int num) { return {num, *this, false}; }
   
   // Взятие столбца
   MatrixPart operator()(int num) { return {num, *this, true}; }
   
   // Транспонирование матрицы
   Matrix operator~() {
       Matrix newM(size, 0);
       newM.values = new int *[size];
       for (int i = 0; i < size; ++i) {
           newM.values[i] = new int[size];
           for (int j = 0; j < size; ++j)
               newM.values[i][j] = values[j][i];
       }
       return newM;
   }
   
   std::ostream &operator<<(std::ostream &ostream, const Matrix &matrix) {
       for (int i = 0; i < matrix.size; ++i) {
           for (int j = 0; j < matrix.size; ++j)
               ostream << matrix.values[i][j] << " ";
           ostream << std::endl;
       }
       return ostream;
   }
   
   ~Matrix() {
       usedPart->make_inaccessible();
       deleteMem();
   }};



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

//    Matrix km(size, k);
//    Matrix *matrixArr = new Matrix[MATRIX_COUNT];
//    for (int i = 0; i < MATRIX_COUNT; ++i) {
//        matrixArr[i] = Matrix(size, std::cin);
//    }
//
//    std::cout << (matrixArr[A] + matrixArr[B] * ~matrixArr[C] + km) * ~matrixArr[D];
//    delete[] matrixArr;
    Matrix *a = new Matrix(size, std::cin);
    std::cout << *a;
    MatrixPart b = (*a)(1);
//    std::cout << a;
    MatrixPart c = (*a)[1];
    delete a;
//    a(2)[1] = -1000;
//    std::cout << a;
//    std::cout << a(1, 1);

}
