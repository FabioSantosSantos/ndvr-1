#ifndef IBF_INDEXER_HPP
#define IBF_INDEXER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>


enum IndexerType {vector, map, boolean_vector};

class AbstractIndexer {
public:
    AbstractIndexer(int size, int hashFunctions)
        : m_size(size), m_hashFunctions(hashFunctions), m_count(0) {}

    AbstractIndexer(int size, int hashFunctions, int count)
        : m_size(size), m_hashFunctions(hashFunctions), m_count(count) {}

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

    bool is_empty() const {
        return m_count == 0;
    }


    int get_count() const {
        return m_count;
    }

    int getSize() {return m_size;}
    int getHashFunctions() {return m_hashFunctions;}
    int getCount() {return m_count;}

    virtual std::vector<size_t> getNumbers() = 0;
    virtual ~AbstractIndexer() = default;

protected:
    int m_size;
    int m_hashFunctions;
    int m_count;  

    virtual void _insert(const std::string& element, int index) = 0;
    virtual void _remove(const std::string& element, int index) = 0;
    virtual bool _contains(const std::string& element, int index) = 0;

    size_t get_index(const std::string& element, int hashFunctionNumber){
        auto hashValue = hash(element, hashFunctionNumber);
        return hashValue % m_size;
        
    }

    uint64_t hash(const std::string& element, int index) {
            return std::hash<std::string>{}(element + std::to_string(index));
    }

};

class BooleanVectorIndexer: public AbstractIndexer{
public:
    std::vector<size_t> getNumbers(){
    	std::vector<size_t> numbers;

    	for(bool number: m_numbers){
    		numbers.push_back(1 ? number : 0);
    	}

        return numbers;
    }

    bool operator==(BooleanVectorIndexer &obj){
        int objSize = obj.get_count();
        int thisSize = this->get_count();

        if (thisSize != objSize)
          return false;

        auto other_numbers = obj.getNumbers();
        for (int i = 0; i < objSize; i++) {
          if (m_numbers[i] != other_numbers[i]) {
            return false;
          }
        }
        return true;
    }

    BooleanVectorIndexer(int size, int hashFunctions)
        : AbstractIndexer(size, hashFunctions){
            m_numbers = std::vector<bool>(size, 0);
        }

    BooleanVectorIndexer(int size, int hashFunctions, int count, std::vector<size_t> numbers)
        : AbstractIndexer(size, hashFunctions, count) {
            m_numbers = this->convertToBoolean(numbers);
        }
    ~BooleanVectorIndexer(){}
protected:
     std::vector<bool> m_numbers;

    std::vector<bool> convertToBoolean(const std::vector<size_t> &numbers){
    	std::vector<bool> bool_numbers;

    	for(size_t number: numbers){
    		bool_numbers.push_back(number == 1);
    	}

        return bool_numbers;
    }

    void _insert(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_numbers[index_] = true;
    }

    bool _contains(const std::string& element, int index){
        auto index_ = get_index(element, index);
        return m_numbers[index_] == true;
    }

    void _remove(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_numbers[index_] = false;
    }

};

class VectorIndexer: public AbstractIndexer{
public:
    std::vector<size_t> getNumbers(){
        return m_numbers;
    }

    bool operator==(VectorIndexer &obj){
        int objSize = obj.get_count();
        int thisSize = this->get_count();

        if (thisSize != objSize)
          return false;

        auto other_numbers = obj.getNumbers();
        for (int i = 0; i < objSize; i++) {
          if (m_numbers[i] != other_numbers[i]) {
            return false;
          }
        }
        return true;
    }

    VectorIndexer(int size, int hashFunctions)
        : AbstractIndexer(size, hashFunctions){
            m_numbers = std::vector<size_t>(size, 0);
        }

    VectorIndexer(int size, int hashFunctions, int count, std::vector<size_t> numbers)
        : AbstractIndexer(size, hashFunctions, count) {
            m_numbers = std::vector<size_t>();
            
            for (size_t i=0; i<numbers.size(); i++) {
                m_numbers.push_back(numbers[i]);
            }

        }
    ~VectorIndexer(){}
protected:
     std::vector<size_t> m_numbers;

    void _insert(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_numbers[index_] = 1;
    }

    bool _contains(const std::string& element, int index){
        auto index_ = get_index(element, index);
        return m_numbers[index_] == 1;
    }

    void _remove(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_numbers[index_] = 0;
    }

};


class MapIndexer: public AbstractIndexer{
public:
     MapIndexer(int size, int hashFunctions)
        : AbstractIndexer(size, hashFunctions){
            m_numbers =  std::map<size_t, size_t>();
        }

    MapIndexer(int size, int hashFunctions, int count, std::vector<size_t> numbers)
        : AbstractIndexer(size, hashFunctions, count) {
            m_numbers =  std::map<size_t, size_t>();
            
            for (size_t i=0; i<numbers.size(); i++) {
                m_numbers[numbers[i]] = 1;
            }

        }
    ~MapIndexer(){}

    std::vector<size_t> getNumbers(){
        std::vector<size_t> numbers;

        for(auto const& it: m_numbers)
            numbers.push_back(it.first);
        
         return numbers;
    }

    bool operator==(MapIndexer &obj){
        int objSize = obj.get_count();
        int thisSize = this->get_count();

        if (thisSize != objSize)
          return false;

        auto other_numbers = obj.getNumbers();
        auto this_numbers = obj.getNumbers();

        std::set<size_t> others_set(other_numbers.begin(), other_numbers.end());
        std::set<size_t> this_set(this_numbers.begin(), this_numbers.end());

        return others_set == this_set;
    }

protected:
    std::map<size_t, size_t> m_numbers;

    void _insert(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_numbers[index_] = 1;
    }

    bool _contains(const std::string& element, int index){
        auto index_ = get_index(element, index);
        return m_numbers.find(index_) != m_numbers.end();
    }

    void _remove(const std::string& element, int index){
        int index_ = get_index(element, index);
        auto it = m_numbers.find(index_);
        if (it != m_numbers.end())
            m_numbers.erase(it);
    }
   
};


// AbstractIndexer *indexer_factory(int size, int hashFunctions, IndexerType type){
//     if (type == IndexerType::vector)
//         return new VectorIndexer(size, hashFunctions);
//     return new MapIndexer(size, hashFunctions);
// }

// AbstractIndexer *indexer_factory(int size, int hashFunctions, int count, std::vector<size_t> numbers, IndexerType type){
//     if (type == IndexerType::vector)
//         return new VectorIndexer(size, hashFunctions, count, numbers);
//     return new MapIndexer(size, hashFunctions, count, numbers);
// }

#endif // IBF_INDEXER_HPP
