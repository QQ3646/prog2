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

    bool operator==(const Pair<K, V> &pair) { return key == pair.key && value == pair.value; }

    bool operator!=(const Pair<K, V> &pair) { return !(*this == pair); }

    bool is_empty() { return *this == Pair<K, V>(); }
};

template<typename K, typename V>
class HashMap {
protected:
    size_t size;
    float overfull = .6;
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

    void base_rehash(void (*update)(HashMap<K, V> &)) {
        auto *new_pair = new Pair<K, V>[buffer_size];
        for (auto elem: *this) {
            add_to_array(new_pair, buffer_size, elem.get_key(), elem.get_value());
        }
        allocated_mem = buffer_size;

        buffer_size *= 2;
        delete[] values;
        values = new_pair;
        if (update)
            update(*this);
    }

    friend class Iterator;
public:
    HashMap() : size(0) {
        values = new Pair<K, V>[buffer_size]();
        allocated_mem = buffer_size;
        buffer_size *= 2;
    }

    HashMap(float overfill) : HashMap() {
        this->overfull = overfill;
    }

    size_t get_len() {
        return size;
    }

    size_t get_hash(K key) {
        std::hash<K> hash;
        return hash(key) % allocated_mem;
    }

    void rehash() {
        base_rehash(nullptr);
    }

    virtual void add(K key, V value) {
        if (float(size + 1) / allocated_mem >= overfull)
            rehash();
        if (add_to_array(values, allocated_mem, key, value))
            size++;
    }

    V &operator[](K key) {
        size_t start_ind = get_hash(key);
        size_t current_ind = start_ind;
        bool loop = false;
        while (!(loop && current_ind == start_ind)) {
            if (current_ind == allocated_mem) {
                current_ind = 0;
                loop = true;
            }
            if (values[current_ind].is_empty()) {
                if (float(size + 1) / allocated_mem >= overfull)
                    rehash();
                return (values[current_ind] = Pair<K, V>(key, V())).get_value();
            }
            else if (values[current_ind].get_key() == key)
                return values[current_ind].get_value();
            current_ind++;
        }
    }

    virtual int remove(K key) {
        size_t start_ind = get_hash(key);
        size_t current_ind = start_ind;
        bool loop = false;
        while (!(loop && current_ind == start_ind)) {
            if (current_ind == allocated_mem) {
                current_ind = 0;
                loop = true;
            }
            if (values[current_ind].is_empty())
                return 0;
            else if (values[current_ind].get_key() == key && !values[current_ind].get_flag()) {
                values[current_ind].make_deleted();
                size--;
                return 1;
            }
            current_ind++;
        }
    }

    ~HashMap() {
        delete[] values;
    }

    class Iterator {
        Pair<K, V> *pair;
        size_t pos;
        HashMap<K, V> *hashMap;
    public:
        Iterator() : pair(nullptr), pos(-1), hashMap(nullptr) {}

        Iterator(Pair<K, V> *pair, size_t pos, HashMap<K, V> *hashMap) : pair(pair), pos(pos), hashMap(hashMap) {}

        Iterator(const Iterator &iterator) {
            *this = iterator;
        }

        Iterator &operator=(const Iterator &iterator) {
            if (this == &iterator)
                return *this;
            pair = iterator.pair;
            pos = iterator.pos;
            hashMap = iterator.hashMap;
            return *this;
        }

        Pair<K, V> &operator*() {
            if (pair != nullptr)
                return *pair;
        }

        Pair<K, V> *operator->() {
            if (pair != nullptr)
                return pair;
        }

        bool operator!=(const Iterator &end) {
            if (pair == end.pair)
                return false;
            return true;
        }

        Iterator operator++() {
            int change_ind = 1;
            while (pos + change_ind < hashMap->allocated_mem) {
                if (!(pair + change_ind)->is_empty() && !(pair + change_ind)->get_flag()) {
                    pair = pair + change_ind;
                    pos = pos + change_ind;
                    return *this;
                }
                change_ind++;
            }
            *this = Iterator();
            return *this;
        }
    };

    Iterator find(K key) {
        for(auto iter = begin(); iter != end(); ++iter) {
            if (iter->get_key() == key)
                return iter;
        }
        return Iterator();
    }


    HashMap<K, V>::Iterator begin() {
        size_t current_ind = 0;
        while (true) {
            if (current_ind == allocated_mem)
                return HashMap<K, V>::Iterator();
            if (!values[current_ind].is_empty() && !values[current_ind].get_flag())
                return HashMap<K, V>::Iterator(&values[current_ind], current_ind, this);
            current_ind++;
        }
    }

    HashMap<K, V>::Iterator end() {
        return HashMap<K, V>::Iterator();
    }
};