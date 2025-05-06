#include "../include/MinHash.h"
#include "../include/RabinKarp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

void testMinHashFunctions(const std::string& filePath1, const std::string& filePath2) {
    RabinKarp rabinKarp(5, 11, 997);
    MinHash minHash(100);

    std::ifstream ifs1(filePath1), ifs2(filePath2);
    if (!ifs1 || !ifs2) {
        std::cerr << "Failed to open one of the files: " << filePath1 << " or " << filePath2 << std::endl;
        return;
    }
    std::stringstream buffer1, buffer2;
    buffer1 << ifs1.rdbuf();
    buffer2 << ifs2.rdbuf();
    std::string doc1 = buffer1.str();
    std::string doc2 = buffer2.str();

    // Preprocess documents
    std::string processedDoc1 = rabinKarp.preprocess(doc1);
    std::string processedDoc2 = rabinKarp.preprocess(doc2);

    // Generate k-grams and compute hashes
    auto hashes1 = rabinKarp.computeHashes(rabinKarp.generateKGrams(processedDoc1));
    auto hashes2 = rabinKarp.computeHashes(rabinKarp.generateKGrams(processedDoc2));

    // Compute MinHash signatures
    auto signature1 = minHash.computeSignature(hashes1);
    auto signature2 = minHash.computeSignature(hashes2);

    std::cout << "MinHash Signature 1: ";
    for (const auto& value : signature1) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "MinHash Signature 2: ";
    for (const auto& value : signature2) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Calculate similarity
    double similarity = minHash.calculateSimilarity(signature1, signature2);
    std::cout << "MinHash Similarity: " << similarity * 100 << "%" << std::endl;
}

int main() {
    std::string datasetPath1 = "../dataset/essay1.txt";
    std::string datasetPath2 = "../dataset/essay2.txt";
    std::cout << "\n--- Testing MinHash Functions ---\n";
    testMinHashFunctions(datasetPath1, datasetPath2);
    return 0;
}