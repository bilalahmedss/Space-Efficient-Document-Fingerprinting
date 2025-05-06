#include "../include/RabinKarp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void testRabinKarpFunctions(const std::string& filePath) {
    RabinKarp rabinKarp(5, 11, 997);
    std::ifstream ifs(filePath);
    if (!ifs) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string doc = buffer.str();

    // Preprocess the document
    std::string processedDoc = rabinKarp.preprocess(doc);
    std::cout << "Processed Document: " << processedDoc << std::endl;

    // Generate k-grams
    auto kgrams = rabinKarp.generateKGrams(processedDoc);
    std::cout << "Generated " << kgrams.size() << " k-grams." << std::endl;
    for (const auto& kgram : kgrams) {
        std::cout << kgram << " ";
    }
    std::cout << std::endl;

    // Compute hashes
    auto hashes = rabinKarp.computeHashes(kgrams);
    std::cout << "Computed " << hashes.size() << " unique hashes." << std::endl;
    for (const auto& hash : hashes) {
        std::cout << hash << " ";
    }
    std::cout << std::endl;

    // Test the generateCSV method
    try {
        std::string inputFilePath = "../dataset/essay1.txt";
        std::string outputFilePath = "../test/essay1_output.csv"; // Save in test folder
        rabinKarp.generateCSV(inputFilePath, outputFilePath);
        std::cout << "CSV generated successfully: " << outputFilePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error generating CSV: " << e.what() << std::endl;
    }
}

int main() {
    std::string datasetPath = "../dataset/essay1.txt";
    std::cout << "\n--- Testing Rabin-Karp Functions ---\n";
    testRabinKarpFunctions(datasetPath);
    return 0;
}