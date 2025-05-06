#include "CountingBloomFilter.h"
#include <random>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <fstream>

// Constructor: Initializes the CountingBloomFilter with a given size and hash functions.
CountingBloomFilter::CountingBloomFilter(int size, int numHashFunctions) : size(size), numHashFunctions(numHashFunctions), counts(size, 0) {
    if (size <= 0) {
        throw std::invalid_argument("Size of the Counting Bloom Filter must be greater than zero.");
    }
    initializeHashFunctions();
}

// Initialize the hash functions with random coefficients.
void CountingBloomFilter::initializeHashFunctions() {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(1, std::numeric_limits<int>::max());

    for (int i = 0; i < numHashFunctions; ++i) {
        int a = distribution(generator);
        int b = distribution(generator);
        int p = 2147483647; // A large prime number

        hashFunctions.push_back([a, b, p, this](int x) {
            int hashValue = ((a * x + b) % p);
            return (hashValue < 0 ? hashValue + p : hashValue) % size; // Ensure non-negative index
        });
    }
}

// Utility function for logging
void logMessage(const std::string& message) {
#ifdef CBF_LOGGING
    std::cout << "[LOG] " << message << std::endl;
#endif
}

// Updated add method with logging
void CountingBloomFilter::add(int element) {
    if (element < 0) {
        throw std::invalid_argument("Element value must be non-negative.");
    }

    logMessage("Adding element: " + std::to_string(element));

    for (const auto& hashFunction : hashFunctions) {
        int index = hashFunction(element);
        if (index < 0 || index >= size) {
            throw std::out_of_range("Hash function generated an out-of-bounds index.");
        }
        counts[index]++;
        logMessage("Incremented count at index: " + std::to_string(index));
    }
}

// Updated possiblyContains method with logging
bool CountingBloomFilter::possiblyContains(int element) const {
    if (element < 0) {
        throw std::invalid_argument("Element value must be non-negative.");
    }

    logMessage("Checking if element is possibly contained: " + std::to_string(element));

    for (const auto& hashFunction : hashFunctions) {
        int index = hashFunction(element);
        if (counts[index] == 0) {
            logMessage("Element not found at index: " + std::to_string(index));
            return false;
        }
    }
    logMessage("Element is possibly contained.");
    return true;
}

// Remove an element from the Counting Bloom Filter.
void CountingBloomFilter::remove(int element) {
    for (const auto& hashFunction : hashFunctions) {
        int index = hashFunction(element);
        if (counts[index] > 0) {
            counts[index]--;
        }
    }
}

// Estimate the current false positive rate of the Counting Bloom Filter.
double CountingBloomFilter::estimateFalsePositiveRate(int estimatedNumElements) const {
    // Standard Bloom filter false positive rate formula:
    // (1 - e^(-k * n / m))^k
    // k = numHashFunctions, n = estimatedNumElements, m = size
    if (estimatedNumElements < 0) {
        throw std::invalid_argument("Estimated number of elements must be non-negative.");
    }
    if (size == 0 || numHashFunctions == 0) {
        throw std::runtime_error("Bloom filter size and number of hash functions must be positive.");
    }
    double k = static_cast<double>(numHashFunctions);
    double m = static_cast<double>(size);
    double n = static_cast<double>(estimatedNumElements);
    double expPart = std::exp(-k * n / m);
    double fpRate = std::pow(1.0 - expPart, k);
    logMessage("Estimated false positive rate: " + std::to_string(fpRate));
    return fpRate;
}

// Save the Counting Bloom Filter to a file
void CountingBloomFilter::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) throw std::runtime_error("Failed to open file for writing: " + filename);
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(reinterpret_cast<const char*>(&numHashFunctions), sizeof(numHashFunctions));
    for (int c : counts) ofs.write(reinterpret_cast<const char*>(&c), sizeof(c));
}

// Load the Counting Bloom Filter from a file
bool CountingBloomFilter::loadFromFile(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return false;
    int fileSize = 0, fileNumHash = 0;
    ifs.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
    ifs.read(reinterpret_cast<char*>(&fileNumHash), sizeof(fileNumHash));
    if (fileSize != size || fileNumHash != numHashFunctions) return false;
    for (int& c : counts) ifs.read(reinterpret_cast<char*>(&c), sizeof(c));
    return true;
}