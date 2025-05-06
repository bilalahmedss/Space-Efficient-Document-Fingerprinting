#include "RabinKarp.h"
#include "MinHash.h"
#include "CountingBloomFilter.h"
#include <iostream>
#include <string>
#include <unordered_set>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Entry point of the application.
int main(int argc, char* argv[]) {
    try {
        if (argc >= 2 && std::string(argv[1]) == "--preprocess-dataset") {
            if (argc < 4) throw std::invalid_argument("Usage: --preprocess-dataset <dataset_dir> <cbf_dir>");
            std::string datasetDir = argv[2];
            std::string cbfDir = argv[3];
            std::filesystem::create_directories(cbfDir);
            int k = 5, base = 11, prime = 997, numHashFunctions = 100, bloomFilterSize = 1000;
            RabinKarp rabinKarp(k, base, prime);
            MinHash minHash(numHashFunctions);
            for (const auto& entry : std::filesystem::directory_iterator(datasetDir)) {
                if (!entry.is_regular_file()) continue;
                std::ifstream ifs(entry.path());
                std::stringstream buffer;
                buffer << ifs.rdbuf();
                std::string doc = buffer.str();
                std::cout << "Processing: " << entry.path().filename().string() << std::endl;
                std::string processedDoc = rabinKarp.preprocess(doc);
                auto kgrams = rabinKarp.generateKGrams(processedDoc);
                if (kgrams.empty()) continue;
                auto hashes = rabinKarp.computeHashes(kgrams);
                auto signature = minHash.computeSignature(hashes);
                // Save CBF as before
                // Use a larger CBF size and fewer hash functions for lower false positive rate
                int cbfSize = 10000; // Try 10,000 or higher
                int cbfHashFunctions = 7; // 5-10 is typical for Bloom filters
                CountingBloomFilter bloomFilter(cbfSize, cbfHashFunctions);
                for (auto hash : signature) {
                    if (hash < 0) hash = -hash;
                    bloomFilter.add(hash);
                }
                std::filesystem::path cbfFile = std::filesystem::path(cbfDir) / (entry.path().filename().string() + ".cbf");
                bloomFilter.saveToFile(cbfFile.string());
                // Save MinHash signature
                std::filesystem::path minhashFile = std::filesystem::path(cbfDir) / (entry.path().filename().string() + ".minhash");
                std::ofstream mhofs(minhashFile);
                for (auto v : signature) mhofs << v << " ";
                // Save Rabin-Karp hashes
                std::filesystem::path rkhashFile = std::filesystem::path(cbfDir) / (entry.path().filename().string() + ".rkhash");
                std::ofstream rkofs(rkhashFile);
                for (auto v : hashes) rkofs << v << " ";
                std::cout << "Processed and saved CBF for: " << entry.path().filename().string() << std::endl;
                // After preprocessing each file, estimate and print the CBF false positive rate
                int estimatedNumElements = static_cast<int>(signature.size());
                double fpRate = bloomFilter.estimateFalsePositiveRate(estimatedNumElements);
                std::cout << "Estimated CBF false positive rate for " << entry.path().filename().string() << ": " << fpRate << std::endl;
            }
            std::cout << "Dataset preprocessing complete. CBFs saved to: " << cbfDir << std::endl;
            return 0;
        }
        if (argc >= 3 && std::string(argv[1]) == "--check") {
            std::string inputFile = argv[2];
            std::string cbfDir = argv[3];
            int k = 5, base = 11, prime = 997, numHashFunctions = 100, bloomFilterSize = 1000;
            RabinKarp rabinKarp(k, base, prime);
            MinHash minHash(numHashFunctions);
            std::ifstream ifs(inputFile);
            if (!ifs) throw std::runtime_error("Failed to open input file: " + inputFile);
            std::stringstream buffer;
            buffer << ifs.rdbuf();
            std::string doc = buffer.str();
            std::string processedDoc = rabinKarp.preprocess(doc);
            auto kgrams = rabinKarp.generateKGrams(processedDoc);
            auto hashes = rabinKarp.computeHashes(kgrams);
            auto signature = minHash.computeSignature(hashes);
            CountingBloomFilter inputCBF(bloomFilterSize, numHashFunctions);
            for (auto hash : signature) {
                if (hash < 0) hash = -hash;
                inputCBF.add(hash);
            }
            std::cout << "Comparing input file to dataset...\n";
            // Load input hashes and signature
            std::unordered_set<int> inputHashes = hashes;
            std::vector<int> inputSignature = signature;
            std::vector<std::tuple<std::string, double, double, double>> results;
            for (const auto& entry : std::filesystem::directory_iterator(cbfDir)) {
                if (!entry.is_regular_file()) continue;
                std::string fname = entry.path().filename().string();
                if (fname.size() < 5 || fname.substr(fname.size()-4) != ".cbf") continue;
                std::string baseName = fname.substr(0, fname.size()-4);
                std::unordered_set<int> datasetHashes;
                std::ifstream rkifs(std::filesystem::path(cbfDir) / (baseName + ".rkhash"));
                int v; while (rkifs >> v) datasetHashes.insert(v);
                std::vector<int> datasetSignature;
                std::ifstream mhifs(std::filesystem::path(cbfDir) / (baseName + ".minhash"));
                while (mhifs >> v) datasetSignature.push_back(v);
                double rkSim = rabinKarp.calculateSimilarity(inputHashes, datasetHashes);
                double mhSim = minHash.calculateSimilarity(inputSignature, datasetSignature);
                double combinedSim = 0.5 * rkSim + 0.5 * mhSim;
                results.emplace_back(baseName, rkSim, mhSim, combinedSim);
            }
            // Sort by combined similarity descending
            std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
                return std::get<3>(a) > std::get<3>(b);
            });
            // Print as table
            std::cout << "\n| Document         | Rabin-Karp | MinHash | Combined |\n";
            std::cout <<   "|------------------|------------|---------|----------|\n";
            for (const auto& [name, rk, mh, comb] : results) {
                std::cout << "| " << std::left << std::setw(16) << name
                          << " | " << std::right << std::setw(10) << std::fixed << std::setprecision(2) << rk*100 << "%"
                          << " | " << std::setw(7) << mh*100 << "%"
                          << " | " << std::setw(8) << comb*100 << "%"
                          << " |\n";
            }
            std::cout << std::endl;
            return 0;
        }

        // Validate parameters
        if (argc < 3) {
            throw std::invalid_argument("Usage: <program> <doc1> <doc2>");
        }

        // Example parameters
        int k = 5; // k-gram size
        int base = 11; // Base for Rabin-Karp hashing
        int prime = 997; // Prime number for Rabin-Karp hashing
        int numHashFunctions = 100; // Number of hash functions for MinHash
        int bloomFilterSize = 1000; // Size of the Counting Bloom Filter

        if (k <= 0 || base <= 0 || prime <= 0 || numHashFunctions <= 0 || bloomFilterSize <= 0) {
            throw std::invalid_argument("All parameters must be positive integers.");
        }

        // Initialize components
        RabinKarp rabinKarp(k, base, prime);
        MinHash minHash(numHashFunctions);
        CountingBloomFilter bloomFilter(bloomFilterSize, numHashFunctions);

        // Input documents
        std::string file1 = argv[1];
        std::string file2 = argv[2];
        std::ifstream ifs1(file1), ifs2(file2);
        if (!ifs1) throw std::runtime_error("Failed to open file: " + file1);
        if (!ifs2) throw std::runtime_error("Failed to open file: " + file2);
        std::stringstream buffer1, buffer2;
        buffer1 << ifs1.rdbuf();
        buffer2 << ifs2.rdbuf();
        std::string doc1 = buffer1.str();
        std::string doc2 = buffer2.str();
        if (doc1.empty() || doc2.empty()) {
            throw std::invalid_argument("Input documents must not be empty.");
        }

        // Preprocess documents
        std::string processedDoc1 = rabinKarp.preprocess(doc1);
        std::string processedDoc2 = rabinKarp.preprocess(doc2);

        // Generate k-grams and compute hashes
        auto kgrams1 = rabinKarp.generateKGrams(processedDoc1);
        auto kgrams2 = rabinKarp.generateKGrams(processedDoc2);

        auto hashes1 = rabinKarp.computeHashes(kgrams1);
        auto hashes2 = rabinKarp.computeHashes(kgrams2);

        // Compute MinHash signatures
        auto signature1 = minHash.computeSignature(hashes1);
        auto signature2 = minHash.computeSignature(hashes2);

        // Calculate similarity using Rabin-Karp
        double similarity = rabinKarp.calculateSimilarity(hashes1, hashes2);
        double minHashSimilarity = minHash.calculateSimilarity(signature1, signature2);
        double combinedSimilarity = (0.5 * similarity) + (0.5 * minHashSimilarity);
        std::cout << "\n| Metric           | Value      |\n";
        std::cout <<   "|------------------|-----------|\n";
        std::cout << "| Rabin-Karp       | " << std::setw(9) << std::fixed << std::setprecision(2) << similarity * 100 << "% |\n";
        std::cout << "| MinHash          | " << std::setw(9) << minHashSimilarity * 100 << "% |\n";
        std::cout << "| Combined         | " << std::setw(9) << combinedSimilarity * 100 << "% |\n";
        std::cout << "|------------------|-----------|\n";
        std::cout << "| Verdict          | " << (combinedSimilarity > 0.5 ? "Similar    " : "Not Similar") << "|\n";
        std::cout << "\n";

        // Add signatures to the Counting Bloom Filter (ensure non-negative values)
        for (auto hash : signature1) {
            if (hash < 0) hash = -hash;
            bloomFilter.add(hash);
        }

        // Check if document 2's signature is possibly in the Bloom Filter (ensure non-negative values)
        bool possiblySimilar = true;
        for (auto hash : signature2) {
            if (hash < 0) hash = -hash;
            if (!bloomFilter.possiblyContains(hash)) {
                possiblySimilar = false;
                break;
            }
        }

        std::cout << "Documents are " << (possiblySimilar ? "possibly similar" : "not similar") << " according to the Counting Bloom Filter.\n";

    } // <-- Added missing closing brace for the try block
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

