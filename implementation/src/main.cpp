/**
 * @file main.cpp
 * @brief Main driver file for comparing input text against a dataset of text files.
 * 
 * This file serves as the entry point for the program. It takes an input text file and compares its contents
 * with all files in a dataset folder. The comparison is performed using both Rabin-Karp fingerprint matching and
 * MinHash similarity estimation. The results are printed to the console, indicating whether there is a match
 * between the input file and any files in the dataset.
 * 
 * Functions:
 * - preprocessText(const std::string& text): Preprocesses the input text by removing unwanted characters
 *   and converting it to lowercase.
 * - readTextFromFile(const std::string& filePath): Reads the contents of a text file.
 * - compareInputWithDataset(const std::string& inputFilePath, const std::string& datasetFolderPath): Compares
 *   the input text file with all files in the specified dataset folder using Rabin-Karp and MinHash methods.
 * 
 * The program is run by calling compareInputWithDataset with the path to the input file and the dataset folder.
 */

 #include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>  // For formatting table output
#include <algorithm>
#include "fingerprint.h"
#include "minhash.h"
#include "indexing.h"
#include "utils.h"  // Include the header where preprocessText is declared

// Preprocess text by keeping only alphanumeric characters and converting to lowercase
std::string preprocessText(const std::string& text) {
    std::string cleanText;
    for (char c : text) {
        if (isalnum(c)) {  // Only keep alphanumeric characters
            cleanText += tolower(c);  // Convert to lowercase
        }
    }
    return cleanText;
}

// Function to read text from a .txt file
std::string readTextFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return "";
    }

    std::string line;
    std::string result;

    // Read the file line by line and append it to the result string
    while (std::getline(file, line)) {
        result += line + "\n";
    }

    file.close();
    return result;
}

// Function to compare the input file against all files in the dataset folder
void compareInputWithDataset(const std::string& inputFilePath, const std::string& datasetFolderPath) {
    // Read and preprocess the input text
    std::string inputText = readTextFromFile(inputFilePath);
    if (inputText.empty()) {
        std::cerr << "Failed to read the input file or it's empty!" << std::endl;
        return;
    }

    inputText = preprocessText(inputText);  // Preprocess the input text
    std::cout << "Input file processed successfully: " << inputFilePath << std::endl;

    // Generate fingerprints for the input document
    auto inputFingerprints = createFingerprint(inputText);

    // Print the table header
    std::cout << std::setw(40) << std::left << "Test File" 
              << std::setw(20) << "Rabin-Karp Match" 
              << std::setw(20) << "MinHash Similarity" 
              << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // Compare the input file with each file in the dataset folder
    for (const auto& entry : std::filesystem::directory_iterator(datasetFolderPath)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::string testFilePath = entry.path().string();

            // Read and preprocess the text of the current test file
            std::string testText = readTextFromFile(testFilePath);
            if (testText.empty()) continue;

            testText = preprocessText(testText);

            // Generate fingerprints for the current test file
            auto testFingerprints = createFingerprint(testText);

            // Check if the input file matches the current test file using Rabin-Karp
            bool match = readFingerprint(inputText, testFingerprints);

            // Use MinHash to compare the similarity between the input file and the current test file
            auto inputMinHash = generateMinHashSignatures(inputText, 100, 3);  // 100 hash functions, 3 for nGramSize
            auto testMinHash = generateMinHashSignatures(testText, 100, 3);

            bool similar = compareMinHashSignatures(inputMinHash, testMinHash);

            // Output the results in a table format
            std::cout << std::setw(40) << std::left << testFilePath 
                      << std::setw(20) << (match ? "Match found" : "No match") 
                      << std::setw(20) << (similar ? "Match found" : "No match") 
                      << std::endl;
        }
    }
}
int main() {
    std::string includeFolder = "include";  // Folder containing the input files
    std::string testsFolder = "tests";     // Folder containing the dataset of .txt files

    std::cout << "Starting the comparison process..." << std::endl;

    // Loop through all files in the 'include' folder
    for (const auto& entry : std::filesystem::directory_iterator(includeFolder)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::string inputFile = entry.path().string();  // Get the path of the current input file

            // Print the name of the current file being compared
            std::cout << "Comparing: " << inputFile << std::endl;

            // Call the function to compare the current input file with the files in the 'tests' folder
            compareInputWithDataset(inputFile, testsFolder);
        }
    }

    std::cout << "Comparison complete." << std::endl;

    return 0;
}