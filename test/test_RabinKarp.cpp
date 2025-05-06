#include "../include/RabinKarp.h"
#include <gtest/gtest.h>

// Test case for RabinKarp preprocessing
TEST(RabinKarpTest, Preprocess) {
    RabinKarp rk(5, 11, 101);
    std::string input = "This is a test document.";
    std::string expected = "test document"; // Assuming stopword removal and stemming
    EXPECT_EQ(rk.preprocess(input), expected);
}

TEST(RabinKarpTest, PreprocessSampleDocument) {
    RabinKarp rk(5, 11, 101);
    std::string doc = "This is a test document. This document is only a test.";
    std::string processed = rk.preprocess(doc);
    std::cout << "Preprocessed: " << processed << std::endl;
    // Optionally, assert expected output
    EXPECT_FALSE(processed.empty());
}

// Add more test cases for other RabinKarp methods...

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }