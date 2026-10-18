#pragma once
#include <string>
#include <vector>

struct TestCase {
    std::string id;                 // e.g. "T1", "T2"
    std::string inputPath;          // Full path to T*_in.txt
    std::string expectedOutputPath; // Full path to T*_out.txt
};

namespace TestCaseLoader {

// Scans testCaseDir/<questionId>/ for T*_in.txt / T*_out.txt pairs.
// Returns a sorted vector of TestCase structs.
// Throws std::runtime_error if the directory does not exist or has no valid pairs.
std::vector<TestCase> loadTestCases(const std::string& testCaseDir,
                                    const std::string& questionId);

} // namespace TestCaseLoader
