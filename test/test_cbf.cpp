#include "../include/CountingBloomFilter.h"
#include "../include/RabinKarp.h"
#include "../include/MinHash.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <cmath>

void testCountingBloomFilterFunctions(const std::string& filePath) {
    RabinKarp rabinKarp(5, 11, 997);
    MinHash minHash(100);
    CountingBloomFilter bloomFilter(1000, 100);

    std::ifstream ifs(filePath);
    if (!ifs) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string doc = buffer.str();

    // Preprocess document
    std::string processedDoc = rabinKarp.preprocess(doc);

    // Generate k-grams and compute hashes
    auto hashes = rabinKarp.computeHashes(rabinKarp.generateKGrams(processedDoc));

    // Compute MinHash signature
    auto signature = minHash.computeSignature(hashes);

    // Add signature to Bloom Filter
    for (auto hash : signature) {
        bloomFilter.add(hash);
    }

    // Query Bloom Filter
    bool possiblyContains = true;
    for (auto hash : signature) {
        if (!bloomFilter.possiblyContains(hash)) {
            possiblyContains = false;
            break;
        }
    }
    std::cout << "Bloom Filter verdict for " << filePath << ": " << (possiblyContains ? "Possibly Contains" : "Does Not Contain") << std::endl;

    // Test false positive rate estimation
    int estimatedNumElements = 100; // Example number of elements
    double falsePositiveRate = bloomFilter.estimateFalsePositiveRate(estimatedNumElements);


    // Add multiple elements to the Bloom Filter
    std::vector<int> addedElements;
    for (int i = 1; i <= 100; ++i) { // Add 100 elements to the filter
        addedElements.push_back(i);
        bloomFilter.add(i);
    }

    // Query for a larger set of non-added elements
    std::vector<int> nonAddedElements;
    for (int i = 101; i <= 200; ++i) { // Query 100 elements that were not added
        nonAddedElements.push_back(i);
    }

    int falsePositives = 0;
    for (int element : nonAddedElements) {
        if (bloomFilter.possiblyContains(element)) {
            std::cout << "False positive detected for element: " << element << std::endl;
            falsePositives++;
        }
    }

    // Calculate and display observed false positive rate
    double observedFalsePositiveRate = static_cast<double>(falsePositives) / nonAddedElements.size();
    std::cout << "Observed False Positive Rate: " << observedFalsePositiveRate * 100 << "%" << std::endl;

    // Compare observed rate with estimated rate
    double estimatedRate = bloomFilter.estimateFalsePositiveRate(addedElements.size());
    std::cout << "Estimated False Positive Rate: " << estimatedRate * 100 << "%" << std::endl;

    // Recalculate optimal parameters
    int n = 100; // Number of elements to be added
    double p = 0.01; // Desired false positive rate (1%)
    double ln2 = std::log(2);

    // Optimal filter size (m)
    int m = static_cast<int>(-n * std::log(p) / (ln2 * ln2));

    // Optimal number of hash functions (k)
    int k = static_cast<int>(m / n * ln2);

    std::cout << "Optimal Bloom Filter size (m): " << m << std::endl;
    std::cout << "Optimal number of hash functions (k): " << k << std::endl;

    // Create a new Bloom Filter with optimal parameters
    CountingBloomFilter optimizedBloomFilter(m, k);

    // Add elements to the optimized Bloom Filter
    for (int i = 1; i <= 100; ++i) {
        optimizedBloomFilter.add(i);
    }

    // Query for non-added elements in the optimized Bloom Filter
    int optimizedFalsePositives = 0;
    for (int i = 101; i <= 200; ++i) {
        if (optimizedBloomFilter.possiblyContains(i)) {
            std::cout << "False positive detected for element: " << i << std::endl;
            optimizedFalsePositives++;
        }
    }

    // Calculate and display observed false positive rate for optimized Bloom Filter
    double optimizedObservedFalsePositiveRate = static_cast<double>(optimizedFalsePositives) / 100;
    std::cout << "Observed False Positive Rate (Optimized): " << optimizedObservedFalsePositiveRate * 100 << "%" << std::endl;

    // Compare observed rate with estimated rate for optimized Bloom Filter
    double optimizedEstimatedRate = optimizedBloomFilter.estimateFalsePositiveRate(n);
    std::cout << "Estimated False Positive Rate (Optimized): " << optimizedEstimatedRate * 100 << "%" << std::endl;

    // Add a larger dataset to the optimized Bloom Filter
    for (int i = 1; i <= 1000; ++i) { // Add 1000 elements to the filter
        optimizedBloomFilter.add(i);
    }

    // Query for a larger set of non-added elements
    int largerFalsePositives = 0;
    for (int i = 1001; i <= 2000; ++i) { // Query 1000 elements that were not added
        if (optimizedBloomFilter.possiblyContains(i)) {
            std::cout << "False positive detected for element: " << i << std::endl;
            largerFalsePositives++;
        }
    }

    // Calculate and display observed false positive rate for the larger dataset
    double largerObservedFalsePositiveRate = static_cast<double>(largerFalsePositives) / 1000;
    std::cout << "Observed False Positive Rate (Larger Dataset): " << largerObservedFalsePositiveRate * 100 << "%" << std::endl;

    // Compare observed rate with estimated rate for the larger dataset
    double largerEstimatedRate = optimizedBloomFilter.estimateFalsePositiveRate(1000);
    std::cout << "Estimated False Positive Rate (Larger Dataset): " << largerEstimatedRate * 100 << "%" << std::endl;
}

int main() {
    std::string datasetPath = "../dataset/essay1.txt";
    std::cout << "\n--- Testing Counting Bloom Filter Functions ---\n";
    testCountingBloomFilterFunctions(datasetPath);
    return 0;
}