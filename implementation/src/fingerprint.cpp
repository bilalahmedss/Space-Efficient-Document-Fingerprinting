// - **Uses ASCII values** → Each character is converted to its ASCII number.  
// - **Uses multiplication-based hashing** → Hash grows by multiplying with **PRIME_BASE (31)**.  
// - **Uses a prime base (31)** → Helps spread hash values and reduce collisions.  
// - **Uses a large modulo (10⁹ + 9)** → Prevents overflow and ensures unique hashes.  
// - **Computes the first hash** → Multiplies characters by **BASE** and applies modulo.  
// - **Uses rolling hash updates** → Removes the old character, shifts left, and adds the new one.  
// - **Efficiently slides through the text** → Computes hashes in **O(n) time**.  
// - **Stores fingerprints for all substrings** → Computes hashes for all substrings of a given length.  
// - **Supports dynamic window size** → Allows flexible selection of substring length to generate rolling fingerprints dynamically based on input text.  
// - **Determines dynamic window size** → Uses max(3, text_length / 2) to ensure a minimum size of 3 and adapt based on input length. 

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

const int PRIME_BASE = 31;
const int MODULO = 1e9 + 9;

// Function to preprocess text (convert to lowercase, remove non-alphabetic characters)
string preprocessText(const string &text) {
    string clean_text;
    for (char c : text) {
        if (isalnum(c)) { // Keep only letters and numbers
            clean_text += tolower(c);
        }
    }
    return clean_text;
}

// Function to determine window size dynamically
int determineWindowSize(int text_length) {
    int window_size = max(3, text_length / 2);  // Ensure at least 3 characters
    return window_size;
}

// Function to generate Rabin-Karp fingerprints for substrings of length `window_size`
vector<long long> generateFingerprints(const string &text, int window_size) {
    int n = text.size();
    vector<long long> fingerprints;

    if (n < window_size) return fingerprints; // If text is too short, return empty result

    long long hash_value = 0, power = 1;

    // Compute the first window hash
    for (int i = 0; i < window_size; i++) {
        hash_value = (hash_value * PRIME_BASE + text[i]) % MODULO;
        if (i > 0) power = (power * PRIME_BASE) % MODULO;
    }
    fingerprints.push_back(hash_value);

    // Rolling hash for next windows
    for (int i = window_size; i < n; i++) {
        hash_value = (hash_value - text[i - window_size] * power % MODULO + MODULO) % MODULO;
        hash_value = (hash_value * PRIME_BASE + text[i]) % MODULO;
        fingerprints.push_back(hash_value);
    }

    return fingerprints;
}

// **Unit Test Function**
void testFingerprintGeneration() {
    string text = "qaziandbilalaregoodfriends";
    cout << "Original text: " << text << endl;

    // Preprocess text
    string clean_text = preprocessText(text);
    cout << "Preprocessed text: " << clean_text << endl;

    // Dynamically determine window size
    int window_size = determineWindowSize(clean_text.length());
    cout << "Dynamically chosen window size: " << window_size << endl;

    // Generate fingerprints
    vector<long long> fingerprints = generateFingerprints(clean_text, window_size);

    cout << "Generated fingerprints:\n";
    for (size_t i = 0; i < fingerprints.size(); i++) {
        cout << "Fingerprint " << i + 1 << ": " << fingerprints[i] << endl;
    }
}

int main() {
    testFingerprintGeneration();  // Run unit test
    return 0;
}
