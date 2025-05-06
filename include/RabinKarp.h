#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include <string>
#include <vector>
#include <unordered_set>

// The RabinKarp class implements the Rabin-Karp algorithm for text similarity detection.
// It provides methods for preprocessing text, generating k-grams, computing hashes, and calculating similarity scores.
// The class is designed for efficient and scalable text comparison tasks.

// Class implementing the Rabin-Karp algorithm for text similarity detection.
class RabinKarp {
public:
    RabinKarp(int k, int base, int prime);

    // Preprocess the input text by applying case folding, tokenization, stopword removal, and stemming.
    std::string preprocess(const std::string& text, const std::unordered_set<std::string>& customStopwords = {});

    // Generate k-grams (substrings of length k) from the input text.
    std::vector<std::string> generateKGrams(const std::string& text);

    // Compute Rabin-Karp hashes for the given k-grams.
    std::unordered_set<int> computeHashes(const std::vector<std::string>& kgrams);

    // Calculate similarity between two sets of hashes using Dice's coefficient.
    double calculateSimilarity(const std::unordered_set<int>& hashes1, const std::unordered_set<int>& hashes2);

    // Determine if the similarity score exceeds the given threshold.
    bool isSimilar(double similarityScore, double threshold = 0.4);

    // Generate a CSV file containing the preprocessed document, k-grams, and computed hashes.
    void generateCSV(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    int k; // k-gram size
    int base; // Base for hashing
    int prime; // Prime number for modulo operation
};

#endif // RABIN_KARP_H