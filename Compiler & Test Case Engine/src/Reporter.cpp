#include "Reporter.hpp"
#include "Comparator.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace Reporter {

// ── ANSI color codes (work on Windows 10+ and most terminals) ──
static const char* RESET   = "\033[0m";
static const char* BOLD    = "\033[1m";
static const char* RED     = "\033[31m";
static const char* GREEN   = "\033[32m";
static const char* YELLOW  = "\033[33m";
static const char* CYAN    = "\033[36m";
static const char* MAGENTA = "\033[35m";
static const char* DIM     = "\033[2m";

// ── Box-drawing helpers ──
static const std::string HR    = "==================================================";
static const std::string TOP   = "+" + HR + "+";
static const std::string MID   = "+" + HR + "+";
static const std::string BOT   = "+" + HR + "+";

static std::string statusTagColored(RunStatus status) {
    switch (status) {
        case RunStatus::Passed:            return std::string(GREEN)  + "[PASS]" + RESET;
        case RunStatus::Failed:            return std::string(RED)    + "[FAIL]" + RESET;
        case RunStatus::RuntimeError:      return std::string(MAGENTA)+ "[ RE ]" + RESET;
        case RunStatus::TimeLimitExceeded: return std::string(YELLOW) + "[TLE ]" + RESET;
    }
    return "[????]";
}

static std::string statusTagPlain(RunStatus status) {
    switch (status) {
        case RunStatus::Passed:            return "[PASS]";
        case RunStatus::Failed:            return "[FAIL]";
        case RunStatus::RuntimeError:      return "[ RE ]";
        case RunStatus::TimeLimitExceeded: return "[TLE ]";
    }
    return "[????]";
}

static std::string formatTime(double ms) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(0) << ms << " ms";
    return oss.str();
}

static std::string truncate(const std::string& s, size_t maxLen = 200) {
    if (s.size() <= maxLen) return s;
    return s.substr(0, maxLen) + "...";
}

void printReport(const std::string& questionId,
                 const std::vector<RunResult>& results,
                 const std::string& outputFilePath) {
    int passed = 0, failed = 0, re = 0, tle = 0;
    int total = static_cast<int>(results.size());

    for (const auto& r : results) {
        switch (r.status) {
            case RunStatus::Passed:            passed++; break;
            case RunStatus::Failed:            failed++; break;
            case RunStatus::RuntimeError:      re++;     break;
            case RunStatus::TimeLimitExceeded: tle++;    break;
        }
    }

    // ── 1. Print to Terminal (Colored) ──
    std::cout << "\n" << BOLD << CYAN;
    std::cout << TOP << "\n";
    std::cout << "|           CodeChecker - Test Results             |\n";
    std::cout << MID << RESET << "\n";
    std::cout << "  " << DIM << "Question ID : " << RESET << BOLD << questionId << RESET << "\n";
    std::cout << "  " << DIM << "Source      : input/code.txt" << RESET << "\n";
    std::cout << CYAN << MID << RESET << "\n\n";

    for (const auto& r : results) {
        std::cout << "  " << statusTagColored(r.status) << "  "
                  << BOLD << r.testId << RESET
                  << "  " << DIM << "(" << formatTime(r.timeTakenMs) << ")" << RESET
                  << "\n";

        if (r.status == RunStatus::Failed) {
            std::string expTrimmed = Comparator::trimRight(r.expectedOutput);
            std::string actTrimmed = Comparator::trimRight(r.actualOutput);
            std::cout << "         " << DIM << "Expected: " << RESET << truncate(expTrimmed) << "\n";
            std::cout << "         " << DIM << "Actual  : " << RESET << truncate(actTrimmed) << "\n";
        } else if (r.status == RunStatus::RuntimeError) {
            if (!r.actualOutput.empty()) {
                std::cout << "         " << DIM << r.actualOutput << RESET << "\n";
            }
        }
    }

    std::cout << "\n" << CYAN << MID << RESET << "\n";
    std::cout << "  " << BOLD << "TOTAL: " << total << RESET;
    std::cout << "  |  " << GREEN << BOLD << "PASSED: " << passed << RESET;
    if (failed > 0)
        std::cout << "  |  " << RED << BOLD << "FAILED: " << failed << RESET;
    if (re > 0)
        std::cout << "  |  " << MAGENTA << BOLD << "RE: " << re << RESET;
    if (tle > 0)
        std::cout << "  |  " << YELLOW << BOLD << "TLE: " << tle << RESET;
    std::cout << "\n";
    std::cout << CYAN << BOT << RESET << "\n\n";

    if (passed == total) {
        std::cout << GREEN << BOLD << "  >>> All tests passed! <<<" << RESET << "\n\n";
    } else {
        std::cout << RED << BOLD << "  >>> " << (total - passed)
                  << " test(s) did not pass. <<<" << RESET << "\n\n";
    }

    // ── 2. Write to Output File (Plain text, no ANSI codes) ──
    if (!outputFilePath.empty()) {
        fs::path outPath(outputFilePath);
        if (outPath.has_parent_path()) {
            fs::create_directories(outPath.parent_path());
        }

        std::ofstream outFile(outputFilePath);
        if (outFile.is_open()) {
            outFile << TOP << "\n";
            outFile << "|           CodeChecker - Test Results             |\n";
            outFile << MID << "\n";
            outFile << "  Question ID : " << questionId << "\n";
            outFile << "  Source      : input/code.txt\n";
            outFile << MID << "\n\n";

            for (const auto& r : results) {
                outFile << "  " << statusTagPlain(r.status) << "  "
                        << r.testId << "  (" << formatTime(r.timeTakenMs) << ")\n";

                if (r.status == RunStatus::Failed) {
                    std::string expTrimmed = Comparator::trimRight(r.expectedOutput);
                    std::string actTrimmed = Comparator::trimRight(r.actualOutput);
                    outFile << "         Expected: " << truncate(expTrimmed) << "\n";
                    outFile << "         Actual  : " << truncate(actTrimmed) << "\n";
                } else if (r.status == RunStatus::RuntimeError) {
                    if (!r.actualOutput.empty()) {
                        outFile << "         " << r.actualOutput << "\n";
                    }
                }
            }

            outFile << "\n" << MID << "\n";
            outFile << "  TOTAL: " << total;
            outFile << "  |  PASSED: " << passed;
            if (failed > 0) outFile << "  |  FAILED: " << failed;
            if (re > 0)     outFile << "  |  RE: " << re;
            if (tle > 0)    outFile << "  |  TLE: " << tle;
            outFile << "\n";
            outFile << BOT << "\n\n";

            if (passed == total) {
                outFile << "  >>> All tests passed! <<<\n";
            } else {
                outFile << "  >>> " << (total - passed) << " test(s) did not pass. <<<\n";
            }

            std::cout << "  " << DIM << "[Report saved to: " << outputFilePath << "]" << RESET << "\n\n";
        } else {
            std::cerr << "[Warning] Unable to write output report to: " << outputFilePath << "\n";
        }
    }
}

void printCompileError(const std::string& questionId,
                       const CompileResult& compileResult,
                       const std::string& outputFilePath) {
    // ── 1. Print to Terminal ──
    std::cout << "\n" << BOLD << RED;
    std::cout << TOP << "\n";
    std::cout << "|           CodeChecker - Compile Error            |\n";
    std::cout << MID << RESET << "\n";
    std::cout << "  " << DIM << "Question ID : " << RESET << BOLD << questionId << RESET << "\n";
    std::cout << "  " << DIM << "Source      : input/code.txt" << RESET << "\n";
    std::cout << RED << MID << RESET << "\n\n";

    std::istringstream errStream(compileResult.errorOutput);
    std::string line;
    while (std::getline(errStream, line)) {
        std::cout << "  " << line << "\n";
    }

    std::cout << "\n" << RED << BOT << RESET << "\n\n";

    // ── 2. Write to Output File ──
    if (!outputFilePath.empty()) {
        fs::path outPath(outputFilePath);
        if (outPath.has_parent_path()) {
            fs::create_directories(outPath.parent_path());
        }

        std::ofstream outFile(outputFilePath);
        if (outFile.is_open()) {
            outFile << TOP << "\n";
            outFile << "|           CodeChecker - Compile Error            |\n";
            outFile << MID << "\n";
            outFile << "  Question ID : " << questionId << "\n";
            outFile << "  Source      : input/code.txt\n";
            outFile << MID << "\n\n";

            std::istringstream streamCopy(compileResult.errorOutput);
            while (std::getline(streamCopy, line)) {
                outFile << "  " << line << "\n";
            }

            outFile << "\n" << BOT << "\n";
            std::cout << "  " << DIM << "[Compile error saved to: " << outputFilePath << "]" << RESET << "\n\n";
        }
    }
}

void printError(const std::string& message) {
    std::cout << "\n" << RED << BOLD << "[Error] " << RESET << message << "\n\n";
}

} // namespace Reporter
