#include "iostream"
#include <fstream>
#include "ctype.h"
#include <typeinfo>

class String {
    char *string;
    int size;

    int BUFFER_SIZE, ALLOCATED_SIZE;

    friend std::ofstream &operator<<(std::ofstream &ostream, const String &string);
    friend std::ifstream &operator>>(std::ifstream &istream, String &string);
public:
    String();

    String(char a);

    String(const char *string);

    String(const String &string);

    int get_len() const;

    String &operator=(const char *string);

    String &operator=(const String &string);

    String operator+(const char *string);

    String operator+(const String &string);

    bool operator==(const char *string);

    bool operator==(const String &string);

    char operator[](int n) const;

    String operator()(int a, int b) const;

    ~String();
};
