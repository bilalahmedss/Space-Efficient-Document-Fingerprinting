#ifndef COUNTING_BLOOM_FILTER_H
#define COUNTING_BLOOM_FILTER_H

#include <string>
#include <vector>

class CountingBloomFilter {
private:
    std::vector<int> filter;  // Array of counters for the filter
    int size;                 // Number of slots in the filter
    int hashCount;            // Number of hash functions to use

    // Hash functions (for demonstration purposes)
    int hash1(const std::string& input) const;
    int hash2(const std::string& input) const;

    // Generic hash function with configurable seed for distribution
    int applyHashFunction(const std::string &input, int seed) const;

public:
    // Constructor to initialize the bloom filter with size and number of hash functions
    CountingBloomFilter(int size, int hashCount);

    // Insert a fingerprint into the filter
    void insert(const std::string& fingerprint);

    // Check if a fingerprint is probably in the filter
    bool contains(const std::string& fingerprint) const;

    // Remove a fingerprint from the filter
    void remove(const std::string& fingerprint);

    // For debugging: print the filter's current state
    void printFilter() const;
};

#endif // COUNTING_BLOOM_FILTER_H
