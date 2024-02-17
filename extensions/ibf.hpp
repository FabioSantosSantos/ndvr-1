#include <iostream>
#include <vector>
#include <algorithm>


#ifndef IBF_HPP
#define IBF_HPP


static const int IBF_DEFAULT_SIZE = 50;
static const int IBF_DEFAULT_QTD_HASH_FUNCTIONS = 3;



class InvertibleBloomFilter {
public:

    InvertibleBloomFilter(int size, int hashFunctions)
        : size(size), hashFunctions(hashFunctions), m_bits(size, 0), count(0) {}

    InvertibleBloomFilter(int size, int hashFunctions, int count, std::vector<size_t> bits)
        : size(size), hashFunctions(hashFunctions), m_bits(bits), count(count) {
        	m_bits.assign(bits.begin(), bits.end());
        }

    void insert(const std::string& element) {
        for (int i = 0; i < hashFunctions; ++i) {
            size_t hashValue = hash(element, i);
            m_bits[hashValue % size] ^= hashValue;
        }
        ++count;
    }

    void remove(const std::string& element) {
        for (int i = 0; i < hashFunctions; ++i) {
            size_t hashValue = hash(element, i);
            m_bits[hashValue % size] ^= hashValue;
        }
        --count;
    }

    bool contains(const std::string& element) const {
        for (int i = 0; i < hashFunctions; ++i) {
            size_t hashValue = hash(element, i);
            if ((m_bits[hashValue % size] & hashValue) != hashValue) {
                return false;
            }
        }
        return true;
    }
    
    bool operator==(InvertibleBloomFilter const &obj) {
	    int objSize = obj.get_count();
	    int thisSize = this->get_count();

	    if (thisSize != objSize)
	      return false;

	    for (int i = 0; i < objSize; i++) {
	      if (this->m_bits[i] != obj.m_bits[i]) {
	        return false;
	      }
	    }
	    return true;
  }


    bool is_empty() const {
        return count == 0;
    }


    int get_count() const {
        return count;
    }

    InvertibleBloomFilter *copy(){
    	return new InvertibleBloomFilter(this->size, this->hashFunctions, this->count, this->m_bits);
    }

    std::vector<size_t> getBits(){
    	return m_bits;
    }

protected:
	std::vector<size_t> m_bits = std::vector<size_t>();

private:
    int size;
    int hashFunctions;
    int count;

    size_t hash(const std::string& element, int index) const {
        return std::hash<std::string>{}(element + std::to_string(index));
    }
};

#endif // IBF_HPP
