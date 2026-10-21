#include "Comparator.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

// Removed "static" and added "Comparator::" so Reporter.cpp can access it
std::string Comparator::trimRight(const std::string& str) {
    auto end = str.find_last_not_of(" \t\r\n");

    if (end == std::string::npos) {
        return "";
    }

    return str.substr(0, end + 1);
}

// Trims trailing whitespace from every line and
// removes trailing empty lines from the complete output.
static std::string normalize(const std::string& str) {
    std::istringstream stream(str);
    std::string line;
    std::string result;

    while (std::getline(stream, line)) {
        // Remove trailing spaces, tabs, and carriage returns.
        auto end = line.find_last_not_of(" \t\r");

        if (end != std::string::npos) {
            result += line.substr(0, end + 1);
        }

        result += '\n';
    }

    // Remove trailing newlines and whitespace.
    return Comparator::trimRight(result);
}

// Compares student output with expected output.
bool Comparator::compare(
    const std::string& studentOutput,
    const std::string& expectedOutput
) {
    return normalize(studentOutput) == normalize(expectedOutput);
}