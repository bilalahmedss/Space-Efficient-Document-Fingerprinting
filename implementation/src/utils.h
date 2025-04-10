/**
 * @file utils.h
 * @brief Header file for utility functions.
 * 
 * This file declares utility functions used throughout the project. Currently, it only includes the declaration
 * for the preprocessText function, which is used to clean the input text by removing unwanted characters and
 * converting it to lowercase.
 * 
 * Functions:
 * - preprocessText(const std::string& text): Preprocesses the input text by removing non-alphanumeric characters
 *   and converting it to lowercase.
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>

// Function to preprocess the input text by removing unwanted characters or formatting
std::string preprocessText(const std::string& text);

#endif // UTILS_H
