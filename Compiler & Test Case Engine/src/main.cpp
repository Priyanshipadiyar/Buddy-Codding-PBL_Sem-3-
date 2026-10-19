#include <iostream>
#include <filesystem>
#include "FileReader.hpp"
#include "TestCaseLoader.hpp"
#include "Compiler.hpp"
#include "Runner.hpp"
#include "Reporter.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

int main() {
#ifdef _WIN32
    // Enable ANSI escape codes on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif

    // ── Paths (relative to where the executable is run from) ──
    const std::string codeFile     = "input/code.txt";
    const std::string qidFile      = "input/qidFile.txt";
    const std::string outputFile   = "output/results.txt";
    const std::string testCaseDir  = "testCase";
    const std::string tempDir      = "temp";

    // ── Step 1: Read source code ──
    std::string sourceCode;
    try {
        sourceCode = FileReader::readFile(codeFile);
    } catch (const std::exception& e) {
        Reporter::printError(e.what());
        return 1;
    }

    // ── Step 2: Read question ID ──
    std::string questionId;
    try {
        questionId = FileReader::readQuestionId(qidFile);
    } catch (const std::exception& e) {
        Reporter::printError(e.what());
        return 1;
    }

    // ── Step 3: Load test cases ──
    std::vector<TestCase> testCases;
    try {
        testCases = TestCaseLoader::loadTestCases(testCaseDir, questionId);
    } catch (const std::exception& e) {
        Reporter::printError(e.what());
        return 1;
    }

    std::cout << "\n  Loading " << testCases.size()
              << " test case(s) for " << questionId << "...\n";

    // ── Step 4: Compile ──
    std::cout << "  Compiling input/code.txt...\n";
    CompileResult compileResult = Compiler::compile(sourceCode, tempDir);

    if (!compileResult.success) {
        Reporter::printCompileError(questionId, compileResult, outputFile);
        return 1;
    }

    std::cout << "  Compilation successful.\n";
    std::cout << "  Running test cases...\n";

    // ── Step 5: Run all test cases ──
    std::string exePath = (fs::path(tempDir) / "student_code.exe").string();
    std::vector<RunResult> results;

    for (const auto& tc : testCases) {
        RunResult result = Runner::runTestCase(exePath, tc, tempDir);
        results.push_back(result);
    }

    // ── Step 6: Print report to console and save to output/results.txt ──
    Reporter::printReport(questionId, results, outputFile);

    return 0;
}
