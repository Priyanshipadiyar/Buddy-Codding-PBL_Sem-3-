#pragma once
#include <string>

namespace Comparator {

// Compares actual output vs expected output.
// Trims trailing whitespace and newlines from both strings before comparing.
bool compare(const std::string& actual, const std::string& expected);

// Trims trailing whitespace and newline characters from a string.
std::string trimRight(const std::string& str);

} // namespace Comparator
