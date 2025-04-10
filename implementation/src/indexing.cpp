/**
 * @file indexing.cpp
 * @brief Implementation of the IndexingSystem class.
 * 
 * This file implements the functions declared in the indexing.h header. The IndexingSystem class is responsible
 * for managing an index of fingerprints and their associated metadata. It provides the ability to add, retrieve,
 * and remove fingerprints from the index, as well as output the current state of the index.
 * 
 * @param index: An unordered map that stores document metadata indexed by fingerprints.
 * 
 * Functions:
 * - insert(const std::string& fingerprint, const std::string& metadata): Adds a fingerprint and its
 *   associated metadata to the index.
 * - retrieve(const std::string& fingerprint) const: Retrieves the metadata for a given fingerprint.
 * - remove(const std::string& fingerprint): Removes a fingerprint from the index.
 * - printIndex() const: Prints the current state of the index for debugging purposes.
 */


#include "indexing.h"
#include <iostream>

// Add a fingerprint along with its associated document metadata (like document ID)
void IndexingSystem::insert(const std::string& fingerprint, const std::string& metadata) {
    index[fingerprint] = metadata;
}

// Retrieve the metadata for a document based on its fingerprint
std::string IndexingSystem::retrieve(const std::string& fingerprint) const {
    auto it = index.find(fingerprint);
    if (it != index.end()) {
        return it->second;  // Return metadata if fingerprint exists
    } else {
        return "Not found";  // Return a default message if fingerprint is not found
    }
}

// Remove a fingerprint and its associated metadata from the index
void IndexingSystem::remove(const std::string& fingerprint) {
    index.erase(fingerprint);
}

// Output the contents of the index (useful for debugging)
void IndexingSystem::printIndex() const {
    for (const auto& entry : index) {
        std::cout << "Fingerprint: " << entry.first << ", Metadata: " << entry.second << std::endl;
    }
}
