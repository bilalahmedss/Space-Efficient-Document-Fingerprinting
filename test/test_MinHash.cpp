#define MINHASH_LOGGING
#include "../include/MinHash.h"
#include <gtest/gtest.h>

// Test case for MinHash signature generation
TEST(MinHashTest, ComputeSignature) {
    MinHash mh(100);
    std::unordered_set<int> hashes = {1, 2, 3, 4, 5};
    auto signature = mh.computeSignature(hashes);
    EXPECT_EQ(signature.size(), 100);
}

TEST(MinHashTest, SignatureOnSampleHashes) {
    MinHash mh(10);
    std::unordered_set<int> hashes = {1, 2, 3, 4, 5, 6, 7};
    auto signature = mh.computeSignature(hashes);
    std::cout << "MinHash Signature: ";
    for (int s : signature) std::cout << s << " ";
    std::cout << std::endl;
    EXPECT_EQ(signature.size(), 10);
}

// Add more test cases for other MinHash methods...

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }