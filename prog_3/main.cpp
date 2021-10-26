#include <iostream>
#include "HashMap.h"

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
    int velosiped = 0;
    for(auto elem : map) {
        bool exited = true;
        int velosiped2 = 0;
        for (auto temp_elem : map) {
            if (velosiped2++ > velosiped && temp_elem != elem && elem.get_value() == temp_elem.get_value()) {
                exited = false;
                break;
            }
        }
        if (exited)
            counter++;
        velosiped++;
        // Целых два велосепеда!
    }
    std::cout << map.get_len() << " " << counter;

}

int main() {
    char key_type, value_type;
    std::cin >> key_type >> value_type;
    if (key_type == 'I' && value_type == 'I') map<int, int>();
    else if (key_type == 'I' && value_type == 'D') map<int, double>();
    else if (key_type == 'I' && value_type == 'S') map<int, std::string>();
    else if (key_type == 'D' && value_type == 'I') map<double, int>();
    else if (key_type == 'D' && value_type == 'D') map<double, double>();
    else if (key_type == 'D' && value_type == 'S') map<double, std::string>();
    else if (key_type == 'S' && value_type == 'I') map<std::string, int>();
    else if (key_type == 'S' && value_type == 'D') map<std::string, double>();
    else if (key_type == 'S' && value_type == 'S') map<std::string, std::string>();

}
