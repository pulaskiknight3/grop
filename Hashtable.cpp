/**
 * Hashtable.cpp
 *
 *      Implementation of Hashtable, using chaining.
 *      Interface mostly follows std::unordered_map
 *
 * Nawal Naz Tareque
 * CPSC 223
 * Spring 2023
 *
 */
#include "Hashtable.h"
#include <iostream>
#include <vector>
#include <functional>
// #include <hash>

template <class Value>
Hashtable<Value>::Hashtable() : Hashtable(initial_capacity) {}

template <class Value>
Hashtable<Value>::Hashtable(size_t tablesize)
{
    table.resize(tablesize);  
}

template <class Value>
Value *Hashtable<Value>::insert(const Key &key, const Value &to_insert)
{
    size_t idx = index_of(key); 
    Value *foundVal = find(key);
    if (foundVal != nullptr) {
        *foundVal = to_insert; 
        return foundVal; 
    } 
    if (load_factor() >= max_load_factor) {
        expand(); 
    }
    table[idx].emplace_back(key, to_insert); 
    num_elements++; 
    return &(table[idx].back().val); 
}

template <class Value>
void Hashtable<Value>::expand()
{ 
    size_t new_capacity = table.size() * 2; 
    Hashtable new_table = Hashtable(new_capacity);   
    for (size_t i = 0; i < table.size(); i++) {
        for (size_t j = 0; j < table[i].size(); j++) {
            new_table.insert(table[i][j].key, table[i][j].val); 
        }  
    } 
    swap(*this, new_table);
}

template <class Value>
Value &Hashtable<Value>::at(const Key &key)
{
    Value *to_return = find(key);
    if (to_return == nullptr)
    {
        throw std::runtime_error("Key not in table");
    }
    return *to_return;
}

template <class Value>
const Value &Hashtable<Value>::at(const Key &key) const
{
    const Value *to_return = find(key);
    if (to_return == nullptr)
    {
        throw std::runtime_error("Key not in table");
    }
    return *to_return;
}

template <class Value>
Value *Hashtable<Value>::find(const Key &key)
{
    size_t idx = index_of(key);  
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].key == key) {
            return &(table[idx][i].val); 
        }
    }
    return nullptr; 
}

template <class Value>
const Value *Hashtable<Value>::find(const Key &key) const
{
    size_t idx = index_of(key);  
    for (size_t i = 0; i < table[idx].size(); i++) {
        if (table[idx][i].key == key) {
            return &(table[idx][i].val); 
        }
    }
    return nullptr; 
}

template <class Value>
Value &Hashtable<Value>::operator[](const Key &key)
{ 
    Value *foundVal = find(key); 
    if (foundVal != nullptr) {
        return *foundVal; 
    } 
    Value* defVal = insert(key, Value()); 
    return *defVal; 
}

template <class Value>
size_t Hashtable<Value>::index_of(const Key &key) const
{
    std::hash<std::string> hashCode; 
    return (hashCode(key) % table.size());
}

template <class Value>
size_t Hashtable<Value>::size() const
{
    return num_elements; 
}

template <class Value>
double Hashtable<Value>::load_factor() const
{
    return num_elements / table.size();  
}

// Explicit template specialization for "string"s
template class Hashtable<std::string>;

// Explicit template specialization for "Entry"s
#include "Entry.h"
template class Hashtable<std::vector<Entry>>;
