#ifndef IBF_HPP
#define IBF_HPP

#include "ibf-indexer.hpp"

#include <iostream>
#include <vector>
#include <algorithm>



static const int IBF_DEFAULT_SIZE = 20;
static const int IBF_DEFAULT_QTD_HASH_FUNCTIONS = 3;
//static const IndexerType IBF_DEFAULT_INDEXER_TYPE = IndexerType::vector;
static const IndexerType IBF_DEFAULT_INDEXER_TYPE = IndexerType::boolean_vector;

// Ref: https://github.com/daankolthof/bloomfilter/blob/master/bloomfilter/bloomfilter.h

class InvertibleBloomFilter {
public:

    InvertibleBloomFilter(int size, int hashFunctions){
        	if (IBF_DEFAULT_INDEXER_TYPE == IndexerType::vector){
        		m_indexer = new VectorIndexer(size, hashFunctions);
        	}else if(IBF_DEFAULT_INDEXER_TYPE == IndexerType::map){
    			m_indexer = new MapIndexer(size, hashFunctions);
        	}else{
                m_indexer = new BooleanVectorIndexer(size, hashFunctions);
            }
        }

    InvertibleBloomFilter(int size, int hashFunctions, int count, std::vector<size_t> numbers){
        	if (IBF_DEFAULT_INDEXER_TYPE == IndexerType::vector){
                m_indexer = new VectorIndexer(size, hashFunctions);
            }else if(IBF_DEFAULT_INDEXER_TYPE == IndexerType::map){
                m_indexer = new MapIndexer(size, hashFunctions);
            }else{
                m_indexer = new BooleanVectorIndexer(size, count, hashFunctions, numbers);
            }
        }

    InvertibleBloomFilter(int size, int hashFunctions, int count, std::vector<bool> numbers){
        m_indexer = new BooleanVectorIndexer(size, hashFunctions, count, numbers);
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
        //std::cout << "InvertibleBloomFilter == " << std::endl;
        return this->m_indexer->isEquals(obj.m_indexer);

        // int objSize = obj.get_count();
        // int thisSize = this->get_count();

        // if (thisSize != objSize)
        //   return false;

        // auto other_numbers = obj.getNumbers();
        // auto this_numbers = obj.getNumbers();

        //std::set<size_t> others_set(other_numbers.begin(), other_numbers.end());
        //std::set<size_t> this_set(this_numbers.begin(), this_numbers.end());

      

        // return other_numbers == this_numbers;

    	 // if (IBF_DEFAULT_INDEXER_TYPE == IndexerType::vector)
    	 // 	return *dynamic_cast<VectorIndexer*>(this->m_indexer) == *dynamic_cast<VectorIndexer*>(obj.m_indexer); 
	     // return *dynamic_cast<MapIndexer*>(this->m_indexer) == *dynamic_cast<MapIndexer*>(obj.m_indexer);
  	}

    bool is_empty() const {
        return get_count() == 0;
    }

    int get_count() const {
        return m_indexer->getCount();
    }

    InvertibleBloomFilter *copy(){
    	return new InvertibleBloomFilter(this->m_indexer->getSize(), 
    		this->m_indexer->getHashFunctions(), 
    		this->m_indexer->getCount(), 
    		this->m_indexer->getNumbers());
    }

    std::vector<size_t> getNumbers() const{
    	return m_indexer->getNumbers();
    }

protected:
	AbstractIndexer *m_indexer;

};

#endif // IBF_HPP
