#include "../include/CountingBloomFilter.h"
#include <gtest/gtest.h>
#include <unordered_set>

// Test case for CountingBloomFilter add and contains
TEST(CountingBloomFilterTest, AddAndContains) {
    CountingBloomFilter cbf(1000, 3);
    cbf.add(42);
    EXPECT_TRUE(cbf.possiblyContains(42));
    EXPECT_FALSE(cbf.possiblyContains(43));
}

TEST(CountingBloomFilterTest, AddSampleHashes) {
    CountingBloomFilter cbf(100, 3);
    std::unordered_set<int> hashes = {10, 20, 30, 40, 50};
    for (int h : hashes) cbf.add(h);
    for (int h : hashes) {
        EXPECT_TRUE(cbf.possiblyContains(h));
    }
    std::cout << "Added and checked sample hashes in CountingBloomFilter." << std::endl;
    cbf.estimateFalsePositiveRate(hashes.size());
}

// Add more test cases for other CountingBloomFilter methods...

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }