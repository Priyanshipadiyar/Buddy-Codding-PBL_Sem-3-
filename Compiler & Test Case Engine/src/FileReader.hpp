#pragma once
#include <string>

namespace FileReader {

// Reads the entire contents of a file into a string.
// Throws std::runtime_error if the file cannot be opened.
std::string readFile(const std::string& filePath);

// Reads the question ID from a file (first non-empty trimmed line).
// Throws std::runtime_error if the file is missing or empty.
std::string readQuestionId(const std::string& filePath);

} // namespace FileReader
