#ifndef MIN_HASH_H
#define MIN_HASH_H

#include <vector>
#include <unordered_set>
#include <functional>

// The MinHash class implements the MinHash algorithm for estimating the Jaccard similarity between sets.
// It uses a set of hash functions to compute a compact signature for each set, enabling efficient similarity comparisons.
// The class supports signature computation and similarity calculation.
class MinHash {
public:
    // Constructor to initialize the MinHash with a specified number of hash functions.
    MinHash(int numHashFunctions);

    // Generate MinHash signature for a set of hashes.
    std::vector<int> computeSignature(const std::unordered_set<int>& hashes);

    // Calculate similarity between two MinHash signatures.
    double calculateSimilarity(const std::vector<int>& signature1, const std::vector<int>& signature2);

private:
    int numHashFunctions; // Number of hash functions.
    std::vector<std::function<int(int)>> hashFunctions; // Vector of hash functions.

    // Initialize random hash functions.
    void initializeHashFunctions();
};

#endif // MIN_HASH_H