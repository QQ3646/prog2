#include <iostream>
#include <functional>


#include <functional>

template<typename K, typename V>
class Pair {
    K key;
    V value;
    bool deleted;
public:
    Pair() : key(), value(), deleted(false) {}

    Pair(K key, V value) : key(key), value(value), deleted(false) {}

    K get_key() { return key; }

    V &get_value() { return value; }

    bool get_flag() { return deleted; }

    void make_deleted() {
        deleted = true;
    }

    bool operator==(const Pair<K, V> &pair) {
        if (key == pair.key && value == pair.value)
            return true;
        return false;
    }

    bool operator!=(const Pair<K, V> &pair) {
        return !(*this == pair);
    }

    bool is_empty() {
        if (*this == Pair<K, V>())
            return true;
        return false;
    }
};

template<typename K, typename V>
class HashMap {
protected:
    size_t size;
    const float overfull = .6;
    size_t allocated_mem;
    size_t buffer_size = 10;

    Pair<K, V> *values;

    virtual bool add_to_array(Pair<K, V> *array, size_t size_a, K key, V value) {
        size_t start_ind = get_hash(key);
        size_t current_ind = start_ind;
        while (true) {
            if (current_ind == size_a)
                current_ind = 0;
            if (array[current_ind].get_flag() || array[current_ind].is_empty()) { //Если пара умерла или пустая
                array[current_ind] = Pair<K, V>(key, value);
                return true;
            } else if (array[current_ind].get_key() == key) {
                array[current_ind] = Pair<K, V>(key, value);
                return false;
            }
            current_ind++;
        }
    }

public:
    size_t get_len() {
        return size;
    }

    HashMap() : size(0) {
        values = new Pair<K, V>[buffer_size]();
        allocated_mem = buffer_size;
        buffer_size *= 2;
    }

    size_t get_hash(K key) {
        std::hash<K> hash;
        return hash(key) % allocated_mem;
    }

    void rehash() {
        auto *new_pair = new Pair<K, V>[buffer_size];
        for (auto elem: *this) {
            add_to_array(new_pair, buffer_size, elem.get_key(), elem.get_value());
        }
        allocated_mem = buffer_size;

        buffer_size *= 2;
        delete[] values;
        values = new_pair;
    }

    virtual void add(K key, V value) {
        if (float(size + 1) / allocated_mem >= overfull)
            rehash();
        if (add_to_array(values, allocated_mem, key, value))
            size++;
    }

    V operator[](K key) {
        size_t start_ind = getHash(key);
        size_t current_ind = start_ind;
        bool loop = false;
        while (true) {
            if (current_ind == allocated_mem) {
                current_ind = 0;
                loop = true;
            } else if (loop && current_ind == start_ind)
                throw std::runtime_error("Ключа не существует в хэш-таблице!"); //?
            if (values[current_ind].is_empty())
                return V{};
            else if (values[current_ind].get_key() == key)
                return values[current_ind].get_value();
            current_ind++;
        }
    }

    virtual void remove(K key) {
        size_t start_ind = get_hash(key);
        size_t current_ind = start_ind;
        bool loop = false;
        while (!(loop && current_ind == start_ind)) {
            if (current_ind == allocated_mem) {
                current_ind = 0;
                loop = true;
            }
            if (values[current_ind].is_empty())
                return;
            else if (values[current_ind].get_key() == key && !values[current_ind].get_flag()) {
                values[current_ind].make_deleted();
                size--;
                return;
            }
            current_ind++;
        }
    }

    ~HashMap() {
        delete[] values;
    }

    class Iterator {
        Pair<K, V> *pair;
        size_t pos, alloc_size;
    public:
        Iterator() : pair(nullptr), pos(-1), alloc_size(-1) {}

        Iterator(Pair<K, V> *pair, size_t pos, size_t alloc_size) : pair(pair), pos(pos), alloc_size(alloc_size) {}

        Iterator(const Iterator &iterator) {
            *this = iterator;
        }

        Iterator &operator=(const Iterator &iterator) {
            if (this == &iterator)
                return *this;
            pair = iterator.pair;
            pos = iterator.pos;
            alloc_size = iterator.alloc_size;
            return *this;
        }

        Pair<K, V> &operator*() {
            if (alloc_size != -1)
                return *pair;
        }

        Pair<K, V> &operator->() {
            if (alloc_size != -1)
                return *pair;
        }

        bool operator!=(const Iterator &end) {
            if (pair == end.pair)
                return false;
            return true;
        }

        Iterator operator++() {
            int change_ind = 1;
            while (pos + change_ind < alloc_size) {
                if (!(pair + change_ind)->is_empty() && !((pair + change_ind)->get_flag())) {
                    *this = Iterator(pair + change_ind, pos + change_ind, alloc_size);
                    return *this;
                }
                change_ind++;
            }
            *this = Iterator();
            return *this;
        }
    };

    Iterator begin() {
        size_t current_ind = 0;
        while (true) {
            if (current_ind == allocated_mem)
                return HashMap<K, V>::Iterator();
            if (!values[current_ind].is_empty() && !values[current_ind].get_flag())
                return HashMap<K, V>::Iterator(&values[current_ind], current_ind, allocated_mem);
            current_ind++;
        }
    }

    Iterator end() {
        return Iterator();
    }
};

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
}

void check_key(char key, char value) {
    if (key == 'I') check_value<int>(value);
    else if (key == 'D') check_value<double>(value);
    else if (key == 'S') check_value<std::string>(value);
}

int main() {
    char key_type, value_type;
    std::cin >> key_type >> value_type;
    check_key(key_type, value_type);
}
