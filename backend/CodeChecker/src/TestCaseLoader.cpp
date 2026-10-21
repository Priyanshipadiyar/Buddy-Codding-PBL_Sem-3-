#include "TestCaseLoader.hpp"
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <regex>
#include <set>
#include <map>

namespace fs = std::filesystem;

namespace TestCaseLoader {

std::vector<TestCase> loadTestCases(const std::string& testCaseDir,
                                    const std::string& questionId) {
    fs::path dirPath = fs::path(testCaseDir) / questionId;

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        throw std::runtime_error("Test case directory not found: " + dirPath.string());
    }

    // Regex to match T<number>_in.txt and T<number>_out.txt
    std::regex inPattern(R"(T(\d+)_in\.txt)");
    std::regex outPattern(R"(T(\d+)_out\.txt)");

    std::map<int, std::string> inputFiles;   // testNumber -> full path
    std::map<int, std::string> outputFiles;  // testNumber -> full path

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        std::smatch match;

        if (std::regex_match(filename, match, inPattern)) {
            int num = std::stoi(match[1].str());
            inputFiles[num] = entry.path().string();
        } else if (std::regex_match(filename, match, outPattern)) {
            int num = std::stoi(match[1].str());
            outputFiles[num] = entry.path().string();
        }
    }

    // Pair them up — only include pairs where both _in and _out exist
    std::vector<TestCase> testCases;

    for (const auto& [num, inputPath] : inputFiles) {
        auto it = outputFiles.find(num);
        if (it != outputFiles.end()) {
            TestCase tc;
            tc.id = "T" + std::to_string(num);
            tc.inputPath = inputPath;
            tc.expectedOutputPath = it->second;
            testCases.push_back(tc);
        } else {
            std::cerr << "[Warning] No matching output file for T"
                      << num << "_in.txt — skipping.\n";
        }
    }

    // Sort by test number
    std::sort(testCases.begin(), testCases.end(),
              [](const TestCase& a, const TestCase& b) {
                  // Extract number from id "T<N>"
                  int na = std::stoi(a.id.substr(1));
                  int nb = std::stoi(b.id.substr(1));
                  return na < nb;
              });

    if (testCases.empty()) {
        throw std::runtime_error("No valid test case pairs found in: " + dirPath.string());
    }

    return testCases;
}

} // namespace TestCaseLoader
