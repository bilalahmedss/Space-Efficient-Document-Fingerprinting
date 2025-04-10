#ifndef MINHASH_H
#define MINHASH_H

#include <vector>
#include <string>

// Hash function used to generate MinHash signatures
long long applyHashFunction(const std::string &str, int seed);

// Generate MinHash signatures for a document using a specified number of hash functions
std::vector<long long> generateMinHashSignatures(const std::string &text, int numHashFunctions, int nGramSize = 3);

// Compare two MinHash signatures to estimate their similarity
bool compareMinHashSignatures(const std::vector<long long> &sig1, const std::vector<long long> &sig2);

#endif // MINHASH_H
