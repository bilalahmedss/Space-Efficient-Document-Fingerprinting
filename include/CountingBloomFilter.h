#ifndef COUNTING_BLOOM_FILTER_H
#define COUNTING_BLOOM_FILTER_H

#include <vector>
#include <functional>
#include <string>

// The CountingBloomFilter class provides a probabilistic data structure for set membership testing with support for counting.
// It uses multiple hash functions to map elements to a fixed-size array of counters, allowing for approximate membership queries.
// The class supports adding elements, checking for possible containment, and removing elements.
// Note: False positives are possible, but false negatives are not.

// Class representing a Counting Bloom Filter.
class CountingBloomFilter {
public:
    // Constructor to initialize the Counting Bloom Filter with a given size and number of hash functions.
    CountingBloomFilter(int size, int numHashFunctions);

    // Add an element to the Counting Bloom Filter.
    void add(int element);

    // Check if an element is possibly in the Counting Bloom Filter.
    bool possiblyContains(int element) const;

    // Remove an element from the Counting Bloom Filter.
    void remove(int element);

    // Estimate the current false positive rate of the Counting Bloom Filter.
    // This uses the standard Bloom filter formula: (1 - e^(-kn/m))^k
    // where k = numHashFunctions, m = size, n = estimated number of inserted elements.
    double estimateFalsePositiveRate(int estimatedNumElements) const;

    // Save the Counting Bloom Filter to a file
    void saveToFile(const std::string& filename) const;

    // Load the Counting Bloom Filter from a file
    bool loadFromFile(const std::string& filename);

private:
    int size; // Size of the Bloom filter
    int numHashFunctions; // Number of hash functions
    std::vector<int> counts; // Count array
    std::vector<std::function<int(int)>> hashFunctions; // Vector of hash functions

    // Initialize random hash functions
    void initializeHashFunctions();
};

#endif // COUNTING_BLOOM_FILTER_H