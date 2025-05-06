#include "RabinKarp.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <numeric>
#include <stdexcept>
#include <iostream>
#include <fstream>

// Forward declaration for porterStemmer
std::string porterStemmer(const std::string& word);

// Constructor: Initializes the Rabin-Karp object with k-gram size, base, and prime number.
RabinKarp::RabinKarp(int k, int base, int prime) : k(k), base(base), prime(prime) {}

// Updated preprocess function to allow configurable stopword list.
std::string RabinKarp::preprocess(const std::string& text, const std::unordered_set<std::string>& customStopwords) {
    std::string processed = text;
#ifdef RABINKARP_LOGGING
    std::cout << "[RabinKarp] Original text: " << text << std::endl;
#endif
    std::transform(processed.begin(), processed.end(), processed.begin(), [](unsigned char c) { return std::tolower(c); });
#ifdef RABINKARP_LOGGING
    std::cout << "[RabinKarp] Lowercased: " << processed << std::endl;
#endif

    // Remove punctuation
    processed.erase(std::remove_if(processed.begin(), processed.end(), [](unsigned char c) {
        return std::ispunct(c);
    }), processed.end());
#ifdef RABINKARP_LOGGING
    std::cout << "[RabinKarp] No punctuation: " << processed << std::endl;
#endif

    // Tokenization (split into words)
    std::istringstream iss(processed);
    std::vector<std::string> tokens((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
#ifdef RABINKARP_LOGGING
    std::cout << "[RabinKarp] Tokens: ";
    for (const auto& t : tokens) std::cout << t << " ";
    std::cout << std::endl;
#endif

    // Improved stopword list (case-insensitive, includes 'this')
    static const std::unordered_set<std::string> defaultStopwords = {
        "and", "the", "is", "in", "at", "of", "on", "a", "to", "this"
    };
    const std::unordered_set<std::string>& stopwords = customStopwords.empty() ? defaultStopwords : customStopwords;
    tokens.erase(std::remove_if(tokens.begin(), tokens.end(), [&stopwords](const std::string& word) {
        return stopwords.find(word) != stopwords.end();
    }), tokens.end());
#ifdef RABINKARP_LOGGING
    std::cout << "[RabinKarp] After stopword removal: ";
    for (const auto& t : tokens) std::cout << t << " ";
    std::cout << std::endl;
#endif

    // Stemming (using updated Porter Stemmer for English)
    for (auto& token : tokens) {
        std::string before = token;
        token = porterStemmer(token);
#ifdef RABINKARP_LOGGING
        std::cout << "[RabinKarp] Stemmed '" << before << "' to '" << token << "'" << std::endl;
#endif
    }

    // Reconstruct the processed text
    processed = std::accumulate(tokens.begin(), tokens.end(), std::string(), [](const std::string& a, const std::string& b) {
        return a.empty() ? b : a + " " + b;
    });
#ifdef RABINKARP_LOGGING
    std::cout << "[RabinKarp] Final processed: " << processed << std::endl;
#endif

    return processed;
}

// Generate k-grams (substrings of length k) from the input text.
std::vector<std::string> RabinKarp::generateKGrams(const std::string& text) {
    std::vector<std::string> kgrams;
    for (size_t i = 0; i + k <= text.size(); ++i) {
        kgrams.push_back(text.substr(i, k));
    }
    return kgrams;
}

// Add error handling for empty documents and invalid input.
std::unordered_set<int> RabinKarp::computeHashes(const std::vector<std::string>& kgrams) {
    if (kgrams.empty()) {
        throw std::invalid_argument("Input k-grams cannot be empty.");
    }

    std::unordered_set<int> hashes;
    int base = 11; // Default base value
    int prime = 10000019; // Default prime value

    for (const auto& kgram : kgrams) {
        int hash = 0;
        for (size_t i = 0; i < kgram.size(); ++i) {
            hash = (hash * base + kgram[i]) % prime;
        }

        if (hash < 0) {
            hash = -hash; // Ensure non-negative hash values
        }

        // Handle potential hash collisions by ensuring unique entries
        while (hashes.find(hash) != hashes.end()) {
            hash = (hash + 1) % prime; // Linear probing to resolve collision
        }

        hashes.insert(hash);
    }
    return hashes;
}

// Calculate similarity between two sets of hashes using Dice's coefficient.
double RabinKarp::calculateSimilarity(const std::unordered_set<int>& hashes1, const std::unordered_set<int>& hashes2) {
    int intersectionSize = 0;
    for (const auto& hash : hashes1) {
        if (hashes2.find(hash) != hashes2.end()) {
            ++intersectionSize;
        }
    }
    return (2.0 * intersectionSize) / (hashes1.size() + hashes2.size());
}

// Determine if the similarity score exceeds the given threshold.
bool RabinKarp::isSimilar(double similarityScore, double threshold) {
    return similarityScore >= threshold;
}

// Updated Porter Stemmer implementation to handle edge cases and improve accuracy.
std::string porterStemmer(const std::string& word) {
    if (word.length() <= 2) return word; // Too short to stem

    std::string stem = word;

    // List of exceptions: words that should not be stemmed
    static const std::unordered_set<std::string> exceptions = {"has", "free", "will"};
    if (exceptions.find(word) != exceptions.end()) {
        return word; // Return the word as is if it's in the exceptions list
    }

    auto hasVowel = [](const std::string& str) {
        return str.find_first_of("aeiou") != std::string::npos;
    };

    // Step 1a: Remove 's' suffixes only if it is not part of a meaningful word
    if (stem.back() == 's' && hasVowel(stem.substr(0, stem.length() - 1))) {
        if (stem.length() > 1 && stem[stem.length() - 2] == 'e') {
            stem.pop_back(); // Remove 's' from "es"
        } else {
            stem.pop_back(); // Remove trailing 's'
        }
    }

    // Step 1b: Remove 'ed' or 'ing' suffixes only if it is not part of a meaningful word
    if (stem.length() > 2 && stem.substr(stem.length() - 2) == "ed" && hasVowel(stem.substr(0, stem.length() - 2))) {
        stem = stem.substr(0, stem.length() - 2);
    } else if (stem.length() > 3 && stem.substr(stem.length() - 3) == "ing" && hasVowel(stem.substr(0, stem.length() - 3))) {
        stem = stem.substr(0, stem.length() - 3);
    }

    // Step 1c: Prevent removal of double consonants unless meaningful
    if (stem.length() > 1 && stem[stem.length() - 1] == stem[stem.length() - 2] && hasVowel(stem.substr(0, stem.length() - 1))) {
        stem.pop_back();
    }

    // Step 2: Handle double consonants (e.g., "running" -> "run")
    if (stem.length() > 1 && stem[stem.length() - 1] == stem[stem.length() - 2]) {
        stem.pop_back();
    }

    // Step 3: Additional rules for common suffixes
    if (stem.length() > 3 && stem.substr(stem.length() - 3) == "ion") {
        stem = stem.substr(0, stem.length() - 3);
    }

    return stem;
}

void RabinKarp::generateCSV(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    if (!inputFile) {
        throw std::runtime_error("Failed to open input file: " + inputFilePath);
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string document = buffer.str();

    // Preprocess the document
    std::string processedDoc = preprocess(document);

    // Generate k-grams
    auto kgrams = generateKGrams(processedDoc);

    // Compute hashes
    auto hashes = computeHashes(kgrams);

    // Write to CSV
    std::ofstream outputFile(outputFilePath);
    if (!outputFile) {
        throw std::runtime_error("Failed to open output file: " + outputFilePath);
    }

    outputFile << "Preprocessed Document," << processedDoc << "\n";
    outputFile << "K-Grams,Hashes\n";
    for (size_t i = 0; i < kgrams.size(); ++i) {
        outputFile << kgrams[i] << ",";
        if (i < hashes.size()) {
            auto it = hashes.begin();
            std::advance(it, i);
            outputFile << *it;
        }
        outputFile << "\n";
    }

    outputFile.close();
}