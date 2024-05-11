#ifndef IBF_INDEXER_HPP
#define IBF_INDEXER_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <bitset>


enum IndexerType {vector, map, boolean_vector};
static const int DEFAULT_SIZE = 64; //32 ou 64

class SingleNumberIndexer {
public:
    SingleNumberIndexer(int size, int hashFunctions)
        : m_size(size), m_hashFunctions(hashFunctions), m_count(0) {
            m_bitset =  std::bitset<DEFAULT_SIZE>();
        }

    SingleNumberIndexer(int size, int hashFunctions, int count, size_t number)
        : m_size(size), m_hashFunctions(hashFunctions), m_count(count) {
            m_bitset =  std::bitset<DEFAULT_SIZE>(number);
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

    bool isEquals(SingleNumberIndexer *other){
        return m_bitset ==other->m_bitset and getCount() == other->getCount();
    }

protected:
    int m_size;
    int m_hashFunctions;
    int m_count;
    std::bitset<DEFAULT_SIZE> m_bitset;  

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
    virtual bool isEquals(AbstractIndexer *other) = 0;

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

    	for(bool number: m_bool_numbers){
    		numbers.push_back(number ? 1: 0);
    	}

        return numbers;
    }

    bool isEquals(AbstractIndexer *other){
    	BooleanVectorIndexer *otherBoolVector = dynamic_cast<BooleanVectorIndexer*>(other);

    	for (int i=0; i < this->getSize();i++){
    		if(m_bool_numbers[i] != otherBoolVector->m_bool_numbers[i]){
    			return false;
    		}
    	}

    	return true;

    }


    BooleanVectorIndexer(int size, int hashFunctions)
        : AbstractIndexer(size, hashFunctions){
            m_bool_numbers = std::vector<bool>(size, false);
        }

    BooleanVectorIndexer(int size, int hashFunctions, int count, std::vector<size_t> numbers)
        : AbstractIndexer(size, hashFunctions, count) {
            m_bool_numbers = this->convertToBoolean(numbers);
        }

     BooleanVectorIndexer(int size, int hashFunctions, int count, std::vector<bool> numbers)
        : AbstractIndexer(size, hashFunctions, count) {
            m_bool_numbers = std::vector<bool>();

            for (bool number: numbers) {
            	m_bool_numbers.push_back(number);
          }
        }
    ~BooleanVectorIndexer(){}
protected:
     std::vector<bool> m_bool_numbers;

    std::vector<bool> convertToBoolean(const std::vector<size_t> &numbers){
    	std::vector<bool> bool_numbers;

    	for(size_t number: numbers){
    		bool_numbers.push_back(number == 1);
    	}

        return bool_numbers;
    }

    void _insert(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_bool_numbers[index_] = true;
    }

    bool _contains(const std::string& element, int index){
        auto index_ = get_index(element, index);
        return m_bool_numbers[index_];
    }

    void _remove(const std::string& element, int index){
        int index_ = get_index(element, index);
        m_bool_numbers[index_] = false;
    }

};

class VectorIndexer: public AbstractIndexer{
public:
	bool isEquals(AbstractIndexer *other){return false;}
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

    bool isEquals(AbstractIndexer *other){return false;}

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
