#include "String.h"


int get_len_of_NP(const char *string) {
    int i = 0;
    for (; string[i]; ++i);
    return i;
}

//String operator+()

String::String() : size(0), BUFFER_SIZE(2), ALLOCATED_SIZE(1) {
    string = new char[1];
    string[0] = '\0';
}

String::String(char a) {
    size = 1;
    string = new char[2];
    string[0] = a;
    string[1] = '\0';
    ALLOCATED_SIZE = 2;
    BUFFER_SIZE = 2;
}

String::String(const char *string) : String() {
    *this = string;
}

String::String(const String &string) : String(string.string) {}

std::ifstream &operator>>(std::ifstream &istream, String &string) {
    delete[] string.string;

    string.string = new char[2];
    string.string[0] = '\0';
    string.ALLOCATED_SIZE = 2;
    string.size = 0;
    string.BUFFER_SIZE = 4;

    char temp = (char) istream.get();
    int current_index = 0;
    while (temp != EOF) {
        if (current_index + 1 >= string.ALLOCATED_SIZE) {
            char *tempstr = new char[string.size + 1];
            int i = 0;
            for (; i < string.size; ++i)
                tempstr[i] = string.string[i];
            tempstr[i] = '\0';

            delete[] string.string;
            string.string = new char[string.BUFFER_SIZE];
            string.ALLOCATED_SIZE = string.BUFFER_SIZE;

            for (i = 0; tempstr[i]; ++i)
                string.string[i] = tempstr[i];
            string.string[i] = '\0';

            delete[] tempstr;

            string.BUFFER_SIZE *= 2;
        }
        string.string[current_index++] = temp;
        string.string[current_index] = '\0';
        string.size++;
        temp = (char) istream.get();
    }
    return istream;
}

int String::get_len() const { return size; }

String &String::operator=(const char *string) {
    size = get_len_of_NP(string);
    delete[] this->string;
    this->string = new char[size + 1];
    for (int i = 0; i < size; ++i) { this->string[i] = string[i]; }
    this->string[size] = '\0';
    ALLOCATED_SIZE = size + 1;
    BUFFER_SIZE = ALLOCATED_SIZE * 2;
    return *this;
}

String &String::operator=(const String &string) {
    if (this == &string)
        return *this;
    *this = string.string;  // Здесь и в других местах, я вызываю методы от поля string, тем самым теряя выгоду от того, что размер уже посчитан
    // Вроде бы и убираю копипасту, но тем не менее любая оперция, даже с уже известым размером становится O(n)
    // Мне кажется это слишком спорный момент, вроде и кода меньше, но зато время сильно возрастает впустую
    return *this;
}

String String::operator+(const char *string) {
    int newSize = size + get_len_of_NP(string);
    char *newStr = new char[newSize + 1];
    for (int i = 0; i < size; ++i)
        newStr[i] = this->string[i];
    for (int i = 0; string[i]; ++i)
        newStr[size + i] = string[i];
    newStr[newSize] = '\0';
    return {newStr};
}

String String::operator+(const String &string) {
    return *this + string.string;
}


bool String::operator==(const char *string) {
    if (size != get_len_of_NP(string))
        return false;
    for (int i = 0; i < size; ++i) {
        if (this->string[i] != string[i])
            return false;
    }
    return true;
}

bool String::operator==(const String &string) { return *this == string.string; }

std::ofstream &operator<<(std::ofstream &ostream, const String &string) {
    for (int i = 0; i < string.size; ++i)
        ostream.put(string.string[i]);
    return ostream;
}

char String::operator[](int n) const {
    if (n > size)
        throw std::runtime_error("Invalid index of string");
    return string[n];
}

String String::operator()(int a, int b) const {
    if (a >= size or a > b)
        throw std::runtime_error("Incorrect [a; b] indexes");
    int newSize = b - a;
    char *temp = new char[newSize + 1];
    for (int i = a; i < b; ++i) {
        temp[i - a] = string[i];
    }
    temp[newSize] = '\0';
    String str(temp);
    delete[] temp;
    return {str};
}

String::~String() {
    delete[] string;
}