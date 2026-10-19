#include "Comparator.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace Comparator {

std::string trimRight(const std::string& str) {
    auto end = str.find_last_not_of(" \t\r\n");
    if (end == std::string::npos) {
        return "";
    }
    return str.substr(0, end + 1);
}

// Trims trailing whitespace from each line, then trims trailing empty lines.
// This gives a robust comparison that ignores trailing whitespace differences.
static std::string normalize(const std::string& str) {
    std::istringstream stream(str);
    std::string line;
    std::string result;

    while (std::getline(stream, line)) {
        // Trim trailing whitespace from each line (including \r)
        auto end = line.find_last_not_of(" \t\r");
        if (end != std::string::npos) {
            result += line.substr(0, end + 1);
        }
        result += "\n";
    }

    // Trim trailing newlines from the whole output
    return trimRight(result);
}

bool compare(const std::string& actual, const std::string& expected) {
    return normalize(actual) == normalize(expected);
}

} // namespace Comparator
