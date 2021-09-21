#include <iostream>
#include "iostream"

class Matrix {
    int size;
    int **values;
public:
    Matrix() {
        this->size = 0;
        this->values = nullptr;
    }

    Matrix(int size) : Matrix(size, 1) {}

    Matrix(int size, int value) : size(size) {
        this->values = new int *[size];
        for (int i = 0; i < size; i++) {
            this->values[i] = new int[size];
            for (int j = 0; j < size; ++j)
                this->values[i][j] = 0;
            this->values[i][i] = value;
        }
    }

// Мне не нравится эти одинаковые конструкторы, но что поделать
// Пытался в верхнем вызвать нижний конструктор, но это не сработало так как мне надо
    Matrix(int size, int *values) : size(size) {
        this->values = new int *[size];
        for (int i = 0; i < size; i++) {
            this->values[i] = new int[size];
            for (int j = 0; j < size; ++j)
                this->values[i][j] = 0;
            this->values[i][i] = values[i];
        }
    }

    Matrix(int size, int **values) : size(size) {
        this->values = values;
    }

// Получение минора
    Matrix getMinor(int a, int b) {
        int **newValues = new int *[size - 1];
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
    Matrix operator+(const Matrix &a) const {
        if (size != a.size) {
            std::cout << "Ошибка: размеры матриц не совпадают.";
            exit(1);
        }
        int **newValues = new int *[size];
        for (int i = 0; i < size; i++) {
            newValues[i] = new int[size];
            for (int j = 0; j < size; j++)
                newValues[i][j] = this->values[i][j] + a.values[i][j];
        }
        return Matrix(size, newValues);
    }

// Сравнение матриц
    bool operator==(const Matrix &b) {
        if (size != b.size)
            return false;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (this->values[i][j] != b.values[i][j])
                    return false;
        return true;
    }

// Вывод матрицы на экран
    void operator+() {
        for (int i = 0; i < this->size; ++i) {
            for (int j = 0; j < this->size; ++j) {
                printf("%d ", this->values[i][j]);
            }
            printf("\n");
        }
    }

// Умножение матриц
    Matrix operator*(const Matrix &b) const {
        int **newValues = new int *[size];
        for (int i = 0; i < size; i++) {
            newValues[i] = new int[size];
            for (int j = 0; j < size; j++) {
                int tempSum = 0;
                for (int k = 0; k < size; ++k) {
                    tempSum += this->values[i][k] * b.values[k][j];
                }
                newValues[i][j] = tempSum;
            }
        }
        return Matrix(size, newValues);
    }

// Взятие строки
    int *operator[](int num) {
        return values[num];
    }

// Взятие столбца
    int *operator()(int num) {

    }

// Транспонирование матрицы
    Matrix operator~() {
        int **newValues = new int *[size];
        for (int i = 0; i < size; ++i) {
            newValues[i] = new int[size];
            for (int j = 0; j < size; ++j)
                newValues[i][j] = this->values[j][i];
        }
        return Matrix(size, newValues);
    }

    ~Matrix() {
        for (int i = 0; i < size; ++i)
            delete[] values[i];
        delete[] values;
    }
};

enum namesOfMatrix {
    A, B, C, D,
};

int main() {
    int size;
    scanf("%d", &size);
    int k;
    scanf("%d", &k);
    int ***buffer = new int **[4];
    for (int i = 0; i < 4; ++i) {
        buffer[i] = new int *[size];
        for (int j = 0; j < size; ++j)
            buffer[i][j] = new int[size];
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int l = 0; l < size; ++l) {
                scanf("%d", &buffer[i][j][l]);
            }
        }
    }
    // Пытался через массив матриц, но выдавало сигсег, так что лучше так
    Matrix a = Matrix(size, buffer[0]);
    Matrix b = Matrix(size, buffer[1]);
    Matrix c = Matrix(size, buffer[2]);
    Matrix d = Matrix(size, buffer[3]);
    Matrix km = Matrix(size, k);
    +((a + b * (~c) + km) * ~d);
}
