#pragma once
#include <string>
#include "TestCaseLoader.hpp"

// Possible outcomes for a single test run
enum class RunStatus {
    Passed,
    Failed,
    RuntimeError,
    TimeLimitExceeded
};

struct RunResult {
    std::string testId;
    RunStatus status;
    std::string actualOutput;
    std::string expectedOutput;
    double timeTakenMs;
};

namespace Runner {

// Runs the compiled executable against a single test case.
// Pipes the test input via stdin redirection and captures stdout.
// exePath: path to the compiled student binary
// testCase: the test case to run
// tempDir: scratch directory for intermediate files
// timeLimitMs: max allowed execution time in milliseconds (default 5000)
RunResult runTestCase(const std::string& exePath,
                      const TestCase& testCase,
                      const std::string& tempDir,
                      int timeLimitMs = 5000);

} // namespace Runner
