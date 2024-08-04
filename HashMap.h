#pragma once
#include <iostream>
#include <functional> 

template<typename Key, typename Value>
class Hashmap {
private:
    struct HashNode {
        Key key;
        Value value;
        bool isOccupied;

        HashNode() : isOccupied(false) {}
    };

    HashNode* table;
    int capacity;
    int size;

    int hashFunction(const Key& key) const {
        std::hash<Key> hashFn;
        return hashFn(key) % capacity;
    }

    void resize() {
        int oldCapacity = capacity;
        capacity *= 2;
        HashNode* newTable = new HashNode[capacity];

        for (int i = 0; i < capacity; ++i) {
            newTable[i].isOccupied = false;
        }

        for (int i = 0; i < oldCapacity; ++i) {
            if (table[i].isOccupied) {
                int newIndex = hashFunction(table[i].key);
                int j = 0;
                while (newTable[(newIndex + j * j) % capacity].isOccupied) {
                    j++;
                }
                newTable[(newIndex + j * j) % capacity] = table[i];
            }
        }

        delete[] table;
        table = newTable;
    }

public:
    Hashmap(int cap = 1000) : capacity(cap), size(0) {
        table = new HashNode[capacity];
        for (int i = 0; i < capacity; ++i) {
            table[i].isOccupied = false;
        }
    }

    ~Hashmap() {
        delete[] table;
    }

    void insert(const Key& key, const Value& value) {
        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % capacity].isOccupied) {
            i++;
        }

        table[(index + i * i) % capacity].key = key;
        table[(index + i * i) % capacity].value = value;
        table[(index + i * i) % capacity].isOccupied = true;
        size++;

        if (static_cast<float>(size) / capacity > 0.75) {
            resize();
        }
    }

    Value* search(const Key& key) {
        int index = hashFunction(key);
        int i = 0;

        while (table[(index + i * i) % capacity].isOccupied) {
            if (table[(index + i * i) % capacity].key == key) {
                return &table[(index + i * i) % capacity].value;
            }
            i++;
        }

        return nullptr;
    }

    void traverse() const {
        for (int i = 0; i < capacity; ++i) {
            if (table[i].isOccupied) {
                std::cout << "Key: " << table[i].key << ", Value: " << table[i].value << std::endl;
            }
        }
    }
};
