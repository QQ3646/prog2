#include "HashMap.h"
#include <utility>
#include <vector>
#include "Matrix/Matrix.h"

template<typename K, typename V>
class MultiHashMap : public HashMap<K, V> {
    //Класс как MatrixPart, только покруче.
    class AVFK {
        std::vector<size_t> vector;
        MultiHashMap<K, V> &hashMap;
        bool hashmap_exist;
        K key;
    public:
        AVFK(K key, MultiHashMap<K, V> &hashMap, std::vector<size_t> vector) : key(key), hashMap(hashMap), vector(std::move(vector)), hashmap_exist(true) {
            hashMap.used_values->push_back(this);
        }

        K get_key() {
            return key;
        }

        V *get_array() {
            if (!hashmap_exist)
                throw std::runtime_error("hashmap is dead.");
            size_t size = hashMap.count_values(key);
            V *array = new V[size];
            for (int i = 0; i < size; ++i) {
                array[i] = hashMap.values[vector[i]].get_value();
            }
            return array;
        }

        V &operator[](size_t index) {
            if (!hashmap_exist)
                throw std::runtime_error("hashmap is dead.");
            return hashMap.values[vector[index]].get_value();
        }

        bool operator==(K key) {
            if (this->key == key)
                return true;
            return false;
        }

        void update() {
            if (hashmap_exist)
                return;
            vector.clear();
            for(int i = 0; i < hashMap.size; i++) {
                if(!hashMap.values[i].is_empty()) {
                    vector.push_back(i);
                }
            }
        }

        void makeInaccessible() {
            hashmap_exist = false;
        }

        ~AVFK() {
            if (hashmap_exist)
                hashMap.used_values->find_and_delete(this);
        }
    };

    List<AVFK> *used_values;

    bool add_to_array(Pair<K, V> *array, size_t size_a, K key, V value) {
        size_t start_ind = this->get_hash(key);
        size_t current_ind = start_ind;
        while (true) {
            if (current_ind == size_a)
                current_ind = 0;
            if (array[current_ind].get_flag() || array[current_ind].is_empty()) { //Если пара умерла или пустая
                array[current_ind] = Pair<K, V>(key, value);
                return true;
            }
            current_ind++;
        }
    }
public:

    // Чисто теоретически, если поиск элемента здесь должен возвращать любой элемент, то самый первый в списке должен подойти
    // Значит можно его не оверрайдить!

    MultiHashMap() : HashMap<K, V>() {
        used_values = new List<AVFK>();
    }

    void add(K key, V value) {
        if (float(this->size + 1) / this->allocated_mem >= this->overfull)
            this->rehash();
        if (add_to_array(this->values, this->allocated_mem, key, value))
            this->size++;
        for (auto &elem: *used_values) {
            if (elem == key) {
                elem.update();
                break;
            }
        }
    }

    void remove(K key) {
        size_t start_ind = this->get_hash(key);
        size_t current_ind = start_ind;
        bool loop = false;
        while (!(loop && current_ind == start_ind)) {
            if (current_ind == this->allocated_mem) {
                current_ind = 0;
                loop = true;
            }
            if (this->values[current_ind].is_empty())
                return;
            else if (this->values[current_ind].get_key() == key && !this->values[current_ind].get_flag()) {
                this->values[current_ind].make_deleted();
                this->size--;
            }
            current_ind++;
        }
        for (auto &elem: *used_values) {
            if (elem == key) {
                elem.update();
                break;
            }
        }
    }

    void rehash() {
        auto *new_pair = new Pair<K, V>[this->buffer_size];
        for (auto elem: *this) {
            add_to_array(new_pair, this->buffer_size, elem.get_key(), elem.get_value());
        }
        this->allocated_mem = this->buffer_size;

        this->buffer_size *= 2;
        delete[] this->values;
        this->values = new_pair;
        for (auto elem: used_values) {
            elem.update();
        }
    }

    size_t count_values(K key) {
        size_t counter = 0;
        size_t start_ind = this->get_hash(key);
        size_t current_ind = start_ind;
        bool loop = false;
        while (!(loop && current_ind == start_ind)) { // Мне оч не нравятся эти while'ы везде, но не знаю чем заменить
                                                      // Возможно foreach бы подошел, но он бы начинал с начала, а не с ключа
            if (current_ind == this->allocated_mem) {
                current_ind = 0;
                loop = true;
            }
            if (this->values[current_ind].is_empty())
                break;
            else if (this->values[current_ind].get_key() == key && !this->values[current_ind].get_flag())
                counter++;
            current_ind++;
        }
        return counter;
    }

    AVFK get_values(K key) {
        std::vector<size_t> indexes;
        for(int i = 0; i < this->allocated_mem; i++) {
            if(!this->values[i].is_empty()) {
                indexes.push_back(i);
            }
        }
        return AVFK(key, *this, indexes);
    }

    ~MultiHashMap() {
        delete used_values;
    }
};

