/**
 * @file fingerprint.cpp
 * @brief Implementation of fingerprint generation and matching functions.
 * 
 * This file implements the functions declared in fingerprint.h. It includes the logic for generating Rabin-Karp
 * fingerprints for documents, creating document fingerprints as sets, and comparing the fingerprints to check for
 * document similarity. The functions also handle preprocessing the input text to remove unwanted characters and
 * format it for fingerprinting.
 * 
 * Functions:
 * - determineWindowSize(int text_length): Calculates the appropriate window size for generating substrings.
 * - generateFingerprints(const std::string& text, int window_size): Generates Rabin-Karp fingerprints for the
 *   document's substrings.
 * - createFingerprint(const std::string& text): Generates a set of unique fingerprints for the document.
 * - readFingerprint(const std::string& text, const std::unordered_set<long long>& storedFingerprints): Compares
 *   the document's fingerprints with a set of stored fingerprints to find matches.
 */
 
#include "fingerprint.h"
#include "minhash.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include "utils.h"

// Determine the appropriate window size based on text length
int determineWindowSize(int text_length) {
    return std::max(3, text_length / 2);  // Ensure window size is at least 3 characters
}

// Generate Rabin-Karp fingerprints for substrings of a specified window size
std::vector<long long> generateFingerprints(const std::string &text, int window_size) {
    int n = text.size();
    std::vector<long long> fingerprints;

    if (n < window_size) return fingerprints;  // If text is too short, return empty vector

    long long hash_value = 0, power = 1;

    // Compute the hash for the first window
    for (int i = 0; i < window_size; i++) {
        hash_value = (hash_value * PRIME_BASE + text[i]) % MODULO;
        if (i > 0) power = (power * PRIME_BASE) % MODULO;
    }
    fingerprints.push_back(hash_value);

    // Rolling hash for subsequent windows
    for (int i = window_size; i < n; i++) {
        hash_value = (hash_value - text[i - window_size] * power % MODULO + MODULO) % MODULO;
        hash_value = (hash_value * PRIME_BASE + text[i]) % MODULO;
        fingerprints.push_back(hash_value);
    }

    return fingerprints;
}

// Generate a unique fingerprint for a document
std::unordered_set<long long> createFingerprint(const std::string &text) {
    std::string clean_text = preprocessText(text);  // Preprocess the text to remove noise
    int window_size = determineWindowSize(clean_text.length());

    // Generate fingerprints for substrings of the cleaned text
    std::vector<long long> fingerprints = generateFingerprints(clean_text, window_size);
    
    // Return a set of unique fingerprints
    return std::unordered_set<long long>(fingerprints.begin(), fingerprints.end());
}

// Check if a document’s fingerprint matches any of the stored fingerprints
bool readFingerprint(const std::string &text, const std::unordered_set<long long> &storedFingerprints) {
    std::string clean_text = preprocessText(text);  // Preprocess the text to remove noise
    int window_size = determineWindowSize(clean_text.length());

    // Generate fingerprints for substrings of the cleaned text
    std::vector<long long> fingerprints = generateFingerprints(clean_text, window_size);
    
    // Check if any generated fingerprint exists in the stored set
    for (long long fingerprint : fingerprints) {
        if (storedFingerprints.find(fingerprint) != storedFingerprints.end()) {
            return true;  // Match found
        }
    }
    return false;  // No match found
}
