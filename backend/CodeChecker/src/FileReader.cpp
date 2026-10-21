#include "FileReader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace FileReader {

// Security helper to block path traversal (e.g., ../../../etc/passwd)
static void validatePath(const std::string& filePath) {
    try {
        fs::path targetPath = fs::weakly_canonical(filePath);
        fs::path allowedBase = fs::canonical("backend"); // Make sure this matches your root secure folder

        auto mismatch = std::mismatch(allowedBase.begin(), allowedBase.end(), targetPath.begin());
        if (mismatch.first != allowedBase.end()) {
            throw std::runtime_error("Security Exception: Path traversal attempt blocked: " + filePath);
        }
    } catch (const fs::filesystem_error& e) {
        throw std::runtime_error(std::string("Filesystem Error during path validation: ") + e.what());
    }
}

std::string readFile(const std::string& filePath) {
    validatePath(filePath);

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
    validatePath(filePath);

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