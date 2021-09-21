#include <iostream>
#include "cstdio"

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
   MatrixColumn::MatrixColumn(int **pos) : pos(pos) {}
   
   int& MatrixColumn::operator[](int num) {
       return *(pos[num]);
   }
   
   MatrixColumn::~MatrixColumn() {
       delete[] pos;
   }
   
   Matrix() {
       size = 0;
       values = nullptr;
   }
   
   Matrix(int size) : Matrix(size, 1) {}
   
   Matrix(int size, int value) : size(size) {
       values = new int*[size];
       for (int i = 0; i < size; i++) {
           values[i] = new int[size];
           for (int j = 0; j < size; ++j)
               values[i][j] = 0;
           values[i][i] = value;
       }
   }
   
   // Мне не нравится эти одинаковые конструкторы, но что поделать
   // Пытался в верхнем вызвать нижний конструктор, но это не сработало так как мне надо
   Matrix(int size, int* values) : size(size) {
       this->values = new int*[size];
       for (int i = 0; i < size; i++) {
           this->values[i] = new int[size];
           for (int j = 0; j < size; ++j)
               this->values[i][j] = 0;
           this->values[i][i] = values[i];
       }
   }
   
   Matrix(int size, int** values) : size(size) {
       this->values = values;
   }
   
   Matrix& operator=(const Matrix &matrix) {
       if (this == &matrix)
           return *this;
   
       int** newValues = new int*[matrix.size];
       for (int i = 0; i < matrix.size; ++i) {
           newValues[i] = new int[matrix.size];
           for (int j = 0; j < matrix.size; ++j)
               newValues[i][j] = matrix.values[i][j];
       }
   
       for (int i = 0; i < size; ++i) {
           delete[] values[i];
       }
       delete[] values;
   
       size = matrix.size;
       values = newValues;
       return *this;
   }
   
   // Получение минора
   Matrix getMinor(int a, int b) {
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
       return {size - 1, newValues};
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
   Matrix operator+ (const Matrix& a) const {
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
       return {size, newValues};
   }
   
   // Умножение матриц *надо допилить еще умножение на столбец, пожалуй
   Matrix operator*(const Matrix &b) const {
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
       return {size, newValues};
   }
   
   // Взятие строки
   int* operator[](int num) {
       return values[num];
   }
   
   // Взятие столбца
   MatrixColumn operator()(int num) {
       int **start = new int*[size];
       for (int i = 0; i < size; ++i) {
           start[i] = &values[i][num];
       }
       return {start};
   }
   
   // Транспонирование матрицы
   Matrix operator~() {
       int** newValues = new int*[size];
       for (int i = 0; i < size; ++i) {
           newValues[i] = new int[size];
           for (int j = 0; j < size; ++j)
               newValues[i][j] = values[j][i];
       }
       return {size, newValues};
   }
   
   ~Matrix() {
       for (int i = 0; i < size; ++i)
           delete[] values[i];
       delete[] values;
   }
   
   
};

std::ostream &operator<<(std::ostream &ostream, const Matrix &matrix) {
    for (int i = 0; i < matrix.size; ++i) {
        for (int j = 0; j < matrix.size; ++j)
            ostream << matrix.values[i][j] << " ";
        ostream << std::endl;
    }
    return ostream;
}

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

//    for (int i = 0; i < MATRIX_COUNT; ++i)
//        for (int j = 0; j < size; ++j)
//            for (int l = 0; l < size; ++l)
//                std::cin >> buffer[i][j][l];

    // Пытался через массив матриц, но выдавало сигсег, так что лучше так
//    Matrix a = Matrix(size, buffer[0]);
//    Matrix b = Matrix(size, buffer[1]);
//    Matrix c = Matrix(size, buffer[2]);
//    Matrix d = Matrix(size, buffer[3]);
    Matrix km = Matrix(size, k);

//    std::cout << (a + b * ~c + km) * ~d;
}
