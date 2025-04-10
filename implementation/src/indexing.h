#ifndef INDEXING_SYSTEM_H
#define INDEXING_SYSTEM_H

#include <string>
#include <unordered_map>

class IndexingSystem {
private:
    std::unordered_map<std::string, std::string> index;  // Map to associate fingerprints with metadata

public:
    // Add a fingerprint and its associated document metadata (e.g., document ID)
    void insert(const std::string& fingerprint, const std::string& metadata);

    // Retrieve metadata for a document based on its fingerprint
    std::string retrieve(const std::string& fingerprint) const;

    // Remove a fingerprint and its metadata from the index
    void remove(const std::string& fingerprint);

    // Output the current state of the index (for debugging purposes)
    void printIndex() const;
};

#endif // INDEXING_SYSTEM_H
