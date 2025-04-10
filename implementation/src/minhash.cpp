/**
 * @file minhash.cpp
 * @brief Implementation of MinHash functions.
 * 
 * This file implements the functions declared in the minhash.h header. It provides the functionality to generate
 * MinHash signatures for documents using n-grams and hash functions. Additionally, it provides a function to compare
 * two MinHash signatures to estimate the similarity between two documents. The MinHash approach is used to estimate
 * the Jaccard similarity between sets efficiently.
 * 
 * Functions:
 * - applyHashFunction(const std::string& str, int seed): Applies a hash function to the input string and returns
 *   the resulting hash value.
 * - generateMinHashSignatures(const std::string& text, int numHashFunctions, int nGramSize): Generates MinHash
 *   signatures for a given text by applying multiple hash functions to different n-grams.
 * - compareMinHashSignatures(const std::vector<long long>& sig1, const std::vector<long long>& sig2): Compares
 *   the two MinHash signatures and returns whether they are similar based on a threshold of matching hash values.
 */


 
#include "minhash.h"
#include <iostream>
#include <vector>
#include <functional>  // For std::hash
#include <string>
#include <climits>  // For LLONG_MAX

// Hash function used to generate MinHash signatures with a specific seed
long long applyHashFunction(const std::string &str, int seed) {
    std::hash<std::string> hash_fn;
    return hash_fn(str + std::to_string(seed));  // Combine string with seed for unique hash
}

// Generate MinHash signatures for a document using multiple hash functions
std::vector<long long> generateMinHashSignatures(const std::string &text, int numHashFunctions, int nGramSize) {
    std::vector<long long> minHashSignatures;
    int textLength = text.size();

    // Iterate over the number of hash functions
    for (int i = 0; i < numHashFunctions; ++i) {
        long long minHash = LLONG_MAX;  // Start with the largest possible value

        // Iterate over the n-grams in the text
        for (size_t j = 0; j <= textLength - nGramSize; ++j) {
            std::string nGram = text.substr(j, nGramSize);  // Extract the n-gram
            long long hashValue = applyHashFunction(nGram, i);  // Apply hash function to the n-gram
            minHash = std::min(minHash, hashValue);  // Update minHash with the smallest hash value
        }

        minHashSignatures.push_back(minHash);  // Store the result for this hash function
    }

    return minHashSignatures;  // Return the final set of MinHash signatures
}

// Compare two MinHash signatures to estimate the similarity between documents
bool compareMinHashSignatures(const std::vector<long long> &sig1, const std::vector<long long> &sig2) {
    if (sig1.size() != sig2.size()) return false;  // Ensure the signatures are of the same size

    int matchingHashes = 0;
    for (size_t i = 0; i < sig1.size(); ++i) {
        if (sig1[i] == sig2[i]) {
            ++matchingHashes;  // Count the number of matching hash values
        }
    }

    // Consider the documents similar if at least 80% of their hashes match
    return matchingHashes >= sig1.size() * 0.8;  // 80% match threshold
}
