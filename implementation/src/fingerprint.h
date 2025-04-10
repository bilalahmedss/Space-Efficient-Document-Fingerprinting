#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <string>
#include <unordered_set>
#include <vector>

// Constants for the Rabin-Karp hashing algorithm
const int PRIME_BASE = 31;
const int MODULO = 1e9 + 9;

// Function declarations
int determineWindowSize(int text_length);
std::vector<long long> generateFingerprints(const std::string &text, int window_size);
std::unordered_set<long long> createFingerprint(const std::string &text);
bool readFingerprint(const std::string &text, const std::unordered_set<long long> &storedFingerprints);

// Additional functions related to MinHash
std::vector<long long> generateMinHashSignatures(const std::string &text, int numHashFunctions);
bool compareMinHashSignatures(const std::vector<long long> &sig1, const std::vector<long long> &sig2);

#endif // FINGERPRINT_H
