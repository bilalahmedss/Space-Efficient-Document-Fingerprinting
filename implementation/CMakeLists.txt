# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(SpaceEfficientDocumentFingerprinting)

# Set the C++ standard to C++17 (as C++14 doesn't support std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Set the source files for the project
set(SOURCES
    src/main.cpp
    src/countingbloomfilter.cpp
    src/fingerprint.cpp
    src/indexing.cpp
    src/minhash.cpp
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the filesystem library (for GCC 8 and above)
target_link_libraries(${PROJECT_NAME} stdc++fs)

# Optionally, add any specific compiler flags you need
# For example, if you're using GCC or Clang, you might want:
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

# Include Boost for hashing and random number generation (if needed)
# find_package(Boost REQUIRED)

# Link additional libraries if necessary (e.g., Boost, STL)
# target_link_libraries(${PROJECT_NAME} Boost::Boost)
