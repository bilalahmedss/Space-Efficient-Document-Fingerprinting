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

# Remove GTest references
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

# Add individual test executables
add_executable(test_rabin_karp test/test_rabin_karp.cpp)
target_link_libraries(test_rabin_karp PRIVATE DocumentFingerprintingLib)

add_executable(test_cbf test/test_cbf.cpp)
target_link_libraries(test_cbf PRIVATE DocumentFingerprintingLib)

add_executable(test_MinHash test/test_MinHash.cpp)
target_link_libraries(test_MinHash PRIVATE DocumentFingerprintingLib)

