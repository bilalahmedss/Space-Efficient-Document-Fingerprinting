# Document Fingerprinting Project

## Overview
This project implements document fingerprinting techniques using algorithms like Rabin-Karp, MinHash, and Counting Bloom Filter. It is designed for efficient text similarity detection and set membership testing.

## Features
- **Rabin-Karp Algorithm**: For text similarity detection.
- **MinHash Algorithm**: For estimating Jaccard similarity between sets.
- **Counting Bloom Filter**: For probabilistic set membership testing with counting support.

## Build Instructions
1. Ensure you have CMake installed (version 3.10 or higher).
2. Navigate to the project directory and create a build folder:
   ```
   mkdir build
   cd build
   ```
3. Run CMake to configure the project:
   ```
   cmake ..
   ```
4. Build the project:
   ```
   cmake --build .
   ```

## Run Instructions
- To check an input file against the dataset:
  ```
  ./DocumentFingerprinting --check path/to/input.txt ../dataset_cbf
  ```
- To compare two files directly:
  ```
  ./DocumentFingerprinting file1.txt file2.txt
  ```
- To run the tests:
  ```
  ctest
  ```

## Command Line Usage

### 1. Preprocess the Dataset (create CBFs, MinHash, and Rabin-Karp hashes for all dataset files)
```
./DocumentFingerprinting --preprocess-dataset ../dataset ../dataset_cbf
```
- Processes every file in `../dataset/` and creates `.cbf`, `.minhash`, and `.rkhash` files in `../dataset_cbf/`.

### 2. Check an Input File Against the Dataset
```
./DocumentFingerprinting --check path/to/input.txt ../dataset_cbf
```
- Compares `input.txt` to all dataset files using Rabin-Karp, MinHash, and combined similarity.
- Prints a sorted table of similarity scores for each dataset file.

### 3. Classic Two-File Comparison
```
./DocumentFingerprinting file1.txt file2.txt
```
- Compares two files directly using Rabin-Karp, MinHash, and CBF logic.
- Prints similarity scores and a combined similarity score.

### 4. Run All Unit Tests
```
ctest
```
or
```
./DocumentFingerprintingTests
```
- Runs all your Google Test-based unit tests.

## Dependencies
- C++17 or higher
- Google Test (for unit testing)

## Debugging and Bloom Filter Statistics
- The Counting Bloom Filter now provides a method to estimate and log its false positive rate. You can call `estimateFalsePositiveRate(estimatedNumElements)` in your code to print and retrieve the current false positive rate, where `estimatedNumElements` is your estimate of the number of unique elements added to the filter.

## License
This project is licensed under the MIT License.
