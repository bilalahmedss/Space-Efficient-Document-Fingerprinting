cmake_minimum_required(VERSION 3.10)

# Project name
project(DocumentFingerprinting)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/RabinKarp.cpp
    src/MinHash.cpp
    src/CountingBloomFilter.cpp
)

# Create a library target for main code
add_library(DocumentFingerprintingLib STATIC
    src/RabinKarp.cpp
    src/MinHash.cpp
    src/CountingBloomFilter.cpp
)

# Add executable for main app
add_executable(DocumentFingerprinting src/main.cpp)
target_link_libraries(DocumentFingerprinting PRIVATE DocumentFingerprintingLib)

# Enable testing
enable_testing()

# Add Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test files
set(TEST_SOURCES
    test/test_main.cpp
    test/test_CountingBloomFilter.cpp
    test/test_MinHash.cpp
    test/test_RabinKarp.cpp
)

# Add test executable, now linking with main library
add_executable(DocumentFingerprintingTests ${TEST_SOURCES})
target_link_libraries(DocumentFingerprintingTests PRIVATE DocumentFingerprintingLib ${GTEST_LIBRARIES} pthread)

# Add tests
add_test(NAME AllTests COMMAND DocumentFingerprintingTests)