class Matrix {
    int size;
    int** values;
    int* column;
public:
    Matrix();

    Matrix(int size);

    Matrix(int size, int value);

    Matrix(int size, int* values);

    Matrix(int size, int** values);

    Matrix getMinor(int a, int b);

    Matrix operator+(const Matrix& a) const;

    bool operator==(const Matrix& b);

    void operator+();

    Matrix operator*(const Matrix& b) const;

    int* operator[](int num);

    int* operator()(int num);

    Matrix operator~();

    ~Matrix();
};

