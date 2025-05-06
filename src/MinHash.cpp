#include "MinHash.h"
#include <random>
#include <limits>
#include <stdexcept>
#include <iostream>

// Constructor: Initializes the MinHash object with a given number of hash functions.
MinHash::MinHash(int numHashFunctions) : numHashFunctions(numHashFunctions) {
    initializeHashFunctions();
}

// Initialize the hash functions with random coefficients.
void MinHash::initializeHashFunctions() {
    std::mt19937 generator(42); // Fixed seed for deterministic hash functions
    std::uniform_int_distribution<int> distribution(1, std::numeric_limits<int>::max());

    for (int i = 0; i < numHashFunctions; ++i) {
        int a = distribution(generator);
        int b = distribution(generator);
        int p = 2147483647; // A large prime number

        hashFunctions.push_back([a, b, p](int x) {
            return (a * x + b) % p;
        });
    }
}

// Compute the MinHash signature for a given set of hashes.
std::vector<int> MinHash::computeSignature(const std::unordered_set<int>& hashes) {
    if (hashes.empty()) {
        throw std::invalid_argument("Input hash set cannot be empty.");
    }

#ifdef MINHASH_LOGGING
    std::cout << "[MinHash] Computing signature for input hashes: ";
    for (const auto& h : hashes) std::cout << h << " ";
    std::cout << std::endl;
#endif

    std::vector<int> signature(numHashFunctions, std::numeric_limits<int>::max());

    for (const auto& hash : hashes) {
        for (int i = 0; i < numHashFunctions; ++i) {
#ifdef MINHASH_LOGGING
            int hashValue = hashFunctions[i](hash);
            std::cout << "[MinHash] Hash function " << i << " applied to " << hash << " gives " << hashValue << std::endl;
            signature[i] = std::min(signature[i], hashValue);
#else
            signature[i] = std::min(signature[i], hashFunctions[i](hash));
#endif
        }
    }
#ifdef MINHASH_LOGGING
    std::cout << "[MinHash] Final signature: ";
    for (int s : signature) std::cout << s << " ";
    std::cout << std::endl;
#endif

    return signature;
}

// Calculate the Jaccard similarity between two MinHash signatures.
double MinHash::calculateSimilarity(const std::vector<int>& signature1, const std::vector<int>& signature2) {
    int intersectionCount = 0;
    for (size_t i = 0; i < signature1.size(); ++i) {
        if (signature1[i] == signature2[i]) {
            ++intersectionCount;
        }
    }
    return static_cast<double>(intersectionCount) / signature1.size();
}