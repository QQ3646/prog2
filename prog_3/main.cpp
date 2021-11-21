//#include "Matrix/Matrix.h"
//#include "HashMap.h"
#include <iostream>
#include "MultiHashMap.h"

template<typename K, typename V>
void map() {
    HashMap<K, V> map;
    int n;
    std::cin >> n;
    char mode;
    K key;
    for (int i = 0; i < n; ++i) {
        std::cin >> mode >> key;
        if (mode == 'A') {
            V value;
            std::cin >> value;
            map.add(key, value);
        } else if (mode == 'R') map.remove(key);
    }
    int counter = 0;
    for(auto elem : map) {
        bool exited = true;
        bool pos = false; //Пытался с массивом, но лучше имхо с костылем, зато без памяти
        // Наверное здесь лучше развернуть foreach, чтобы отсчет начинать с текущего итератора
        for (auto temp_elem : map) {
            if (temp_elem == elem) {
                pos = true;
                continue;
            }
            if (pos && temp_elem.get_key() != elem.get_key() && elem.get_value() == temp_elem.get_value()) {
                exited = false;
                break;
            }
        }
        if (exited)
            counter++;
    }
    std::cout << map.get_len() << " " << counter;

}

template<typename K>
void check_value(char value) {
    if (value == 'I') map<K, int>();
    else if (value == 'S') map<K, std::string>();
    else if (value == 'D') map<K, double>();
    else if (value == 'M') map<K, Matrix>();
}

void check_key(char key, char value) {
    if (key == 'I') check_value<int>(value);
    else if (key == 'D') check_value<double>(value);
    else if (key == 'S') check_value<std::string>(value);
    else if (key == 'M') check_value<Matrix>(value);
}

int main() {
    MultiHashMap<int, int> a;
    a.add(43, 656);
    a.add(42, 1);
    a.add(41, 111);
    a.add(7, 12);
    a.add(46, 1);
    a.add(47, 111);
    a.add(73, 12);
    for (auto elem: a) {
        std::cout << elem.get_key() << " " << elem.get_value() << "\n";
    }
//    MultiHashMap<int, int> multiHashMap;
//    multiHashMap.add(43, 656);
//    multiHashMap.add(43, 1);
//    multiHashMap.add(43, 111);
//    multiHashMap.add(7, 12);
//    std::cout << multiHashMap.count_values(43) << " ";
//    multiHashMap.get_values(43);
//    std::cout << multiHashMap.get_values(43)[1] << std::endl;
//    int *a = multiHashMap.get_values(43).get_array();
//    size_t count = multiHashMap.count_values(43);
//    for(int i = 0; i < count; i++) {
//        std::cout << a[i] << " ";
//    }
//
//    delete[] a;
//    multiHashMap.get_values(43);
//    multiHashMap.remove(43);
//    char key_type, value_type;
//    std::cin >> key_type >> value_type;
//    check_key(key_type, value_type);
}
