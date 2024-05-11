#ifndef IBF_HPP
#define IBF_HPP

#include "ibf-indexer.hpp"

#include <iostream>
#include <vector>
#include <algorithm>



static const int IBF_DEFAULT_SIZE = 64;
static const int IBF_DEFAULT_QTD_HASH_FUNCTIONS = 12;
//static const IndexerType IBF_DEFAULT_INDEXER_TYPE = IndexerType::vector;
static const IndexerType IBF_DEFAULT_INDEXER_TYPE = IndexerType::boolean_vector;

// Ref: https://github.com/daankolthof/bloomfilter/blob/master/bloomfilter/bloomfilter.h

class InvertibleBloomFilter {
public:

    InvertibleBloomFilter(int size, int hashFunctions){
        m_indexer = new SingleNumberIndexer(size, hashFunctions);
    }

    InvertibleBloomFilter(int size, int hashFunctions, int count, size_t number){
        m_indexer = new SingleNumberIndexer(size, hashFunctions, count, number);
    }


    void insert(const std::string& element) {
    	m_indexer->insert(element);
    }

    void remove(const std::string& element) {
       m_indexer->remove(element);
    }

    bool contains(const std::string& element) const {
    	return m_indexer->contains(element);
    }
    
    bool operator==(InvertibleBloomFilter const &obj) {
        return this->m_indexer->isEquals(obj.m_indexer);
  	}

    int getCount() const {
        return m_indexer->getCount();
    }

    InvertibleBloomFilter *copy(){
    	return new InvertibleBloomFilter(this->m_indexer->getSize(), 
    		this->m_indexer->getHashFunctions(), 
    		this->m_indexer->getCount(), 
    		this->m_indexer->getNumber());
    }

    size_t getNumber() const{
    	return m_indexer->getNumber();
    }

protected:
	SingleNumberIndexer *m_indexer;

};

#endif // IBF_HPP
