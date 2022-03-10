#include <iostream>
#include <optional>
#include <vector>
#include <random>
 
const int kPrimeNumber = 1984533;
 
 
class HashFunction {
private:
    int first_par;
    int second_par;
    int prime_number;
    int size;
public:
    HashFunction() = default;
 
    HashFunction(int _first_par, int _second_par, int _prime_number, int _size) : first_par(_first_par),
    second_par(_second_par), prime_number(_prime_number), size(_size) {}
 
    int getHash(int value) const {
        return (first_par * value + second_par) % prime_number % size;
    }
};
 
bool CheckHashFunction(const std::vector<int>& values, const HashFunction& hashFunction) {
    std::vector<int> bucket;
    bucket.resize(values.size() * values.size(), 0);
    for (const auto& value : values) {
        ++bucket[hashFunction.getHash(value)];
    }
 
    for (auto number : bucket) {
        if (number > 1) {
            return false;
        }
    }
    return true;
}
 
std::vector<int> get_distribution_bucket(const std::vector<int>& values, const HashFunction& hashFunction) {
    std::vector<int> result;
    result.resize(values.size(), 0);
    for (auto value : values) {
        ++result[hashFunction.getHash(value)];
    }
    for (auto number : result) {
        std::cout << number << ' ';
    }
    return result;
 
}
 
bool check_outside_hash_function(const std::vector<int>& distribution_bucket, size_t size) {
    size_t sum_numbers = 0;
    for (auto number : distribution_bucket) {
        sum_numbers += number;
    }
    return sum_numbers <= 4 * size;
}
 
 
HashFunction GenerateHashFunction(size_t numberOfValues, std::mt19937* generator_ptr) {
    std::uniform_int_distribution<size_t> first_distribution(1, kPrimeNumber - 1);
    size_t first_parameter = first_distribution(*generator_ptr);
    std::uniform_int_distribution<size_t> second_distribution(0, kPrimeNumber - 1);
    size_t second_parameter = second_distribution(*generator_ptr);
    return HashFunction(first_parameter, second_parameter, kPrimeNumber, numberOfValues);
}
 
class HashTable {
private:
    std::vector<std::optional<int>> bucket;
 
    size_t size;
    HashFunction hashFunction;
public:
    HashTable() = default;
 
    void Initialize(const std::vector<int>& values) {
        size = values.size() * values.size();
        bucket.resize(size);
        std::random_device dev;
        std::mt19937 generator(dev());
 
        do {
            hashFunction = GenerateHashFunction(size, &generator);
        } while (!CheckHashFunction(values, hashFunction));
 
        for (const auto& value : values) {
            bucket[hashFunction.getHash(value)] = value;
        }
    }
 
    const std::optional<int>& operator[](size_t index) const {
//        if (index >= 0 || index < size) {
            return bucket[index];
//        }
    }
 
    bool find(int value) const {
        return bucket[hashFunction.getHash(value)].has_value() &&
        value == bucket[hashFunction.getHash(value)].value();
    }
 
    size_t get_size() const {
        return size;
    }
 
    ~HashTable() = default;
};
 
 
class FixedSet {
private:
    std::vector<HashTable> fixedSet;
    size_t size;
    HashFunction hashFunction;
public:
    FixedSet(const std::vector<int>& values) : size(values.size()) {
        fixedSet.resize(values.size());
 
        std::random_device dev;
        std::mt19937 generator(dev());
        std::vector<int> distribution_bucket;
        do {
            hashFunction = GenerateHashFunction(size, &generator);
            distribution_bucket = get_distribution_bucket(values, hashFunction);
        } while (!check_outside_hash_function(distribution_bucket, values.size()));
 
        for (size_t i = 0; i < fixedSet.size(); ++i) {
            std::vector<int> elements;
            elements.reserve(distribution_bucket[i]);
            for (auto value : values) {
                if (hashFunction.getHash(value) == i) {
                    elements.push_back(value);
                }
            }
            fixedSet[i].Initialize(elements);
        }
    }
 
    bool Find(int value) const {
        return fixedSet[hashFunction.getHash(value)].find(value);
    }
 
    void Print() const {
        for (const auto& bucket : fixedSet) {
            for (size_t i = 0; i < bucket.get_size(); ++i) {
                if (bucket[i].has_value()) {
                    std::cout << bucket[i].value() << ' ';
                }
            }
            std::cout << std::endl;
        }
    }
 
 
 
 
 
};
 
 
int main() {
    std::vector<int> values = {1, 2, 3, 4, 5 ,6, 7, 8, 9, 10, 11, 12};
    FixedSet fixedSet(values);
    std::cout << fixedSet.Find(3);
    /*fixedSet.Print();*/
 
 
    return 0;
}
