#ifndef IBF_HPP
#define IBF_HPP

#include "ibf-indexer.hpp"

#include <iostream>
#include <vector>
#include <algorithm>


// Ref: https://github.com/daankolthof/bloomfilter/blob/master/bloomfilter/bloomfilter.h

class InvertibleBloomFilter {
public:

    InvertibleBloomFilter(){
        m_indexer = new SingleNumberIndexer();
    }

    InvertibleBloomFilter(size_t number){
        m_indexer = new SingleNumberIndexer(number);
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
        return *this->m_indexer == obj.m_indexer;
  	}

    size_t getCount() const {
        return m_indexer->getCount();
    }

    InvertibleBloomFilter *copy(){
    	return new InvertibleBloomFilter(this->m_indexer->getNumber());
    }

    size_t getNumber() const{
    	return m_indexer->getNumber();
    }

protected:
	SingleNumberIndexer *m_indexer;

};

#endif // IBF_HPP
