#ifndef IBF_INDEXER_HPP
#define IBF_INDEXER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <bitset>

static const int IBF_DEFAULT_SIZE = 64; //32 ou 64
static const int IBF_DEFAULT_BITS_TO_COUNTER = 5;
static const int IBF_DEFAULT_FINAL_BIT_ARRAY_SIZE = IBF_DEFAULT_SIZE - IBF_DEFAULT_BITS_TO_COUNTER;
static const int IBF_DEFAULT_QTD_HASH_FUNCTIONS = 3;


class SingleNumberIndexer {
public:
    SingleNumberIndexer():
        m_count(0){
            m_bitset =  std::bitset<IBF_DEFAULT_SIZE>();
        }

    SingleNumberIndexer(size_t number){
            m_bitset = std::bitset<IBF_DEFAULT_SIZE>(number);
            m_count = getCountIntoNumber();
        }

    void insert(const std::string& element) {
        for (int i = 0; i < m_hashFunctions; i++){
            _insert(element, i);
        }
        ++m_count;
        updateCountIntoNumber();
    }

    void remove(const std::string& element) {
        for (int i = 0; i < m_hashFunctions; i++){
            _remove(element, i);
        }
        --m_count;
        updateCountIntoNumber();
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

    size_t getCount(){return m_count;}

    int getHashFunctions() {return m_hashFunctions;}

    bool operator==(SingleNumberIndexer *other) {
        return getNumber() == other->getNumber() and getCount() == other->getCount();
    }



protected:
    int m_size = IBF_DEFAULT_SIZE;
    int m_real_size = IBF_DEFAULT_FINAL_BIT_ARRAY_SIZE;
    int m_hashFunctions = IBF_DEFAULT_QTD_HASH_FUNCTIONS;
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
        return hashValue % m_real_size + IBF_DEFAULT_BITS_TO_COUNTER;
        
    }

    uint64_t hash(const std::string& element, int index) {
        return std::hash<std::string>{}(element + std::to_string(index));
    }


    size_t getCountIntoNumber() {
        std::bitset<IBF_DEFAULT_BITS_TO_COUNTER> count_bitset;

        for (int i = 0; i < IBF_DEFAULT_BITS_TO_COUNTER; i++){
            count_bitset[i] = m_bitset[i];
        }

        return count_bitset.to_ulong();
    }

    void updateCountIntoNumber(){
        std::bitset<IBF_DEFAULT_BITS_TO_COUNTER> count_bitset(m_count);

        for (int i = 0; i < IBF_DEFAULT_BITS_TO_COUNTER; i++){
            m_bitset[i] = count_bitset[i];
        }
    }


};

#endif // IBF_INDEXER_HPP
