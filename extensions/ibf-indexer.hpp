#ifndef IBF_INDEXER_HPP
#define IBF_INDEXER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <bitset>

static const int IBF_DEFAULT_SIZE = 64; //32 ou 64

class SingleNumberIndexer {
public:
    SingleNumberIndexer(int size, int hashFunctions)
        : m_size(size), m_hashFunctions(hashFunctions), m_count(0) {
            m_bitset =  std::bitset<IBF_DEFAULT_SIZE>();
        }

    SingleNumberIndexer(int size, int hashFunctions, int count, size_t number)
        : m_size(size), m_hashFunctions(hashFunctions), m_count(count) {
            m_bitset =  std::bitset<IBF_DEFAULT_SIZE>(number);
        }

    void insert(const std::string& element) {
        for (int i = 0; i < m_hashFunctions; i++){
            _insert(element, i);
        }
        ++m_count;
    }

    void remove(const std::string& element) {
        for (int i = 0; i < m_hashFunctions; i++){
            _remove(element, i);
        }
         --m_count;
    }

    bool contains(const std::string& element) {
        for (int i = 0; i < m_hashFunctions; i++){
            if (!_contains(element, i))
                return false;
        }
        return true;
    }

    bool isEmpty() const {
        return m_count == 0;
    }

    uint64_t toInt64() const {
        return m_bitset.to_ulong();
    }

    size_t getNumber() const {
        return toInt64();
    }

    int getSize() {return m_size;}
    int getHashFunctions() {return m_hashFunctions;}
    int getCount() {return m_count;}

    bool operator==(SingleNumberIndexer *other) {
        return getNumber() == other->getNumber() and getCount() == other->getCount();
    }



protected:
    int m_size;
    int m_hashFunctions;
    int m_count;
    std::bitset<IBF_DEFAULT_SIZE> m_bitset;  

    void _insert(const std::string& element, int index){
        auto index_ = get_index(element, index);
        m_bitset.set(index_, true);
    }

    bool _contains(const std::string& element, int index){
        auto index_ = get_index(element, index);
        return m_bitset.test(index_);
    }

    void _remove(const std::string& element, int index){
        auto index_ = get_index(element, index);
        m_bitset.set(index_, false);
    }

    size_t get_index(const std::string& element, int hashFunctionNumber){
        auto hashValue = hash(element, hashFunctionNumber);
        return hashValue % m_size;
        
    }

    uint64_t hash(const std::string& element, int index) {
        return std::hash<std::string>{}(element + std::to_string(index));
    }

};

#endif // IBF_INDEXER_HPP
