#include "FileReader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace FileReader {

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    if (content.empty()) {
        throw std::runtime_error("File is empty: " + filePath);
    }

    return content;
}

std::string readQuestionId(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open qidFile: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace from both ends
        auto start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue; // skip blank lines
        auto end = line.find_last_not_of(" \t\r\n");
        std::string trimmed = line.substr(start, end - start + 1);

        if (!trimmed.empty()) {
            return trimmed;
        }
    }

    throw std::runtime_error("qidFile is empty or contains no valid question ID: " + filePath);
}

} // namespace FileReader
