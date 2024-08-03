#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename Key, typename Value>
class HashMap {
private:
    std::vector<std::vector<std::pair<Key, Value>>> table;
    size_t num_buckets;
    size_t size;

    size_t hash_function(const Key &key) const {
        return std::hash<Key>{}(key) % num_buckets;
    }

public:
    HashMap(size_t buckets) : num_buckets(buckets), size(0) {
        table.resize(num_buckets);
    }

    Value& operator[](const Key &key) {
        size_t index = hash_function(key);
        for (auto &pair : table[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        table[index].emplace_back(key, Value());
        ++size;
        return table[index].back().second;
    }

    void insert(const Key &key, const Value &value) {
        size_t index = hash_function(key);
        for (auto &pair : table[index]) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }
        table[index].emplace_back(key, value);
        ++size;
    }

    bool find(const Key &key) const {
        size_t index = hash_function(key);
        for (const auto &pair : table[index]) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }

    Value get(const Key &key) const {
        size_t index = hash_function(key);
        for (const auto &pair : table[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw std::runtime_error("Key not found");
    }

    void traverse(std::vector<Key> &key_vect, std::vector<Value> &value_vect) const {
        for (const auto &bucket : table) {
            for (const auto &pair : bucket) {
                key_vect.push_back(pair.first);
                value_vect.push_back(pair.second);
            }
        }
    }

    size_t get_size() const {
        return size;
    }

    ~HashMap() = default;
};

#endif // HASHMAP_H
