#include <iostream>
#include <vector>
#include <algorithm>


#ifndef IBF_HPP
#define IBF_HPP


static const int IBF_DEFAULT_SIZE = 50;
static const int IBF_DEFAULT_QTD_HASH_FUNCTIONS = 3;

// Ref: https://github.com/daankolthof/bloomfilter/blob/master/bloomfilter/bloomfilter.h

template <typename T> std::string join2(const T &v, const std::string &delim) {
  std::ostringstream s;
  for (const auto &i : v) {
    if (&i != &v[0]) {
      s << delim;
    }
    s << i;
  }
  return s.str();
}

class InvertibleBloomFilter {
public:

    InvertibleBloomFilter(int size, int hashFunctions)
        : size(size), hashFunctions(hashFunctions), count(0) {
        	m_bits = std::vector<size_t>(size, 0);

        	//std::cout << "InvertibleBloomFilter empty constructor"  << std::endl;
        	//std::cout << "InvertibleBloomFilter vector size: "  << m_bits.size() << std::endl;
        }

    InvertibleBloomFilter(int size, int hashFunctions, int count, std::vector<size_t> bits)
        : size(size), hashFunctions(hashFunctions), count(count) {
        	m_bits = std::vector<size_t>();
        	//std::cout << "InvertibleBloomFilter Bits: ";
        	
        	for (size_t i=0; i<bits.size(); i++) {
        		m_bits.push_back(bits[i]);
        		std::cout << bits[i] << ",";
        	}

        	std::cout << std::endl;

        }

    InvertibleBloomFilter(int size, int hashFunctions, int count, std::vector<size_t> *bits)
        : size(size), hashFunctions(hashFunctions), count(count) {
        	m_bits = std::vector<size_t>();
        	//std::cout << "InvertibleBloomFilter Bits: ";
        	for (size_t i=0; i<bits->size(); i++) {
        		m_bits.push_back((*bits)[i]);
        		std::cout << (*bits)[i] << ",";
        	}

        	std::cout << std::endl;
        }

    void insert(const std::string& element) {
    	//std::cout << "InvertibleBloomFilter.insert element: " << element << std::endl;
        for (int i = 0; i < hashFunctions; ++i) {
            auto hashValue = hash(element, i);
            //std::cout << "InvertibleBloomFilter.insert hashValue: " << hashValue << std::endl;
            m_bits[hashValue % size] = 1;
            //std::cout << "InvertibleBloomFilter.insert hashValue mod size: " << hashValue % size << std::endl;
        }
        ++count;
    }

    void remove(const std::string& element) {
        for (int i = 0; i < hashFunctions; ++i) {
            auto hashValue = hash(element, i);
            m_bits[hashValue % size] = 0;
        }
        --count;
    }

    bool contains(const std::string& element) const {
     	//std::cout << "InvertibleBloomFilter.contains element: " << element << std::endl;
        for (int i = 0; i < hashFunctions; ++i) {
            auto hashValue = hash(element, i);
            // std::cout << "InvertibleBloomFilter.contains - hashValue: " << hashValue << std::endl;
            // std::cout << "InvertibleBloomFilter.contains - hashValue % size: " << hashValue % size << std::endl;  
            // std::cout << "InvertibleBloomFilter.contains - bits vector size: " << m_bits.size() << std::endl;   
          	
          	if (m_bits[hashValue % size] != 1) {
            		//std::cout << "InvertibleBloomFilter.contains will return false: " << std::endl;
                	return false;
            	}	

            //std::cout << "InvertibleBloomFilter.contains - bits: [" << join2 (m_bits, ",") << "]" << std::endl;   
           
          
        }
        //std::cout << "InvertibleBloomFilter.contains will return true: " << std::endl;
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

    std::vector<uint64_t> getBits() const{
    	return m_bits;
    }

protected:
	std::vector<size_t> m_bits = std::vector<size_t>();

private:
    int size;
    int hashFunctions;
    int count;

    uint64_t hash(const std::string& element, int index) const {
        return std::hash<std::string>{}(element + std::to_string(index));
    }
};

#endif // IBF_HPP
