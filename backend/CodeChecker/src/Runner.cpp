#include "Runner.hpp"
#include "Comparator.hpp"
#include "FileReader.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

namespace Runner {

#ifdef _WIN32
// Windows implementation using CreateProcess for timeout support
static RunResult runWithTimeout(const std::string& exePath,
                                const std::string& inputPath,
                                const std::string& outputPath,
                                int timeLimitMs) {
    RunResult result;
    result.timeTakenMs = 0;

    // Build command: exe < input > output 2>&1
    std::string cmdLine = "cmd /C \"\"" + exePath + "\" < \"" + inputPath + "\" > \"" + outputPath + "\" 2>&1\"";

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    auto startTime = std::chrono::high_resolution_clock::now();

    // CreateProcess needs a mutable char array
    std::vector<char> cmdBuf(cmdLine.begin(), cmdLine.end());
    cmdBuf.push_back('\0');

    BOOL created = CreateProcessA(
        NULL,           // Application name (use command line)
        cmdBuf.data(),  // Command line
        NULL, NULL,     // Process/thread security
        FALSE,          // Inherit handles
        CREATE_NO_WINDOW, // Creation flags
        NULL, NULL,     // Environment, directory
        &si, &pi
    );

    if (!created) {
        result.status = RunStatus::RuntimeError;
        result.actualOutput = "Failed to create process.";
        return result;
    }

    // Wait for process with timeout
    DWORD waitResult = WaitForSingleObject(pi.hProcess, timeLimitMs);

    auto endTime = std::chrono::high_resolution_clock::now();
    result.timeTakenMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    if (waitResult == WAIT_TIMEOUT) {
        // Process exceeded time limit — kill it
        TerminateProcess(pi.hProcess, 1);
        WaitForSingleObject(pi.hProcess, 1000); // Wait for termination
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        result.status = RunStatus::TimeLimitExceeded;
        result.actualOutput = "[Time Limit Exceeded]";
        return result;
    }

    // Check exit code
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exitCode != 0) {
        result.status = RunStatus::RuntimeError;
        // Try to read whatever output was produced
        std::ifstream outFile(outputPath);
        if (outFile.is_open()) {
            std::stringstream ss;
            ss << outFile.rdbuf();
            result.actualOutput = ss.str();
        }
        if (result.actualOutput.empty()) {
            result.actualOutput = "[Runtime Error — exit code " + std::to_string(exitCode) + "]";
        }
        return result;
    }

    // Success — read output
    result.status = RunStatus::Passed; // Will be updated after comparison
    return result;
}

#else
// Unix/Linux fallback using system()
static RunResult runWithTimeout(const std::string& exePath,
                                const std::string& inputPath,
                                const std::string& outputPath,
                                int timeLimitMs) {
    RunResult result;

    int timeLimitSec = (timeLimitMs + 999) / 1000; // Round up to seconds
    
    // SECURITY FIX: Apply memory limits (256MB) and process limits (64) before running timeout
    std::string cmd = "ulimit -v 262144 && ulimit -u 64 && timeout " + std::to_string(timeLimitSec)
                    + " \"" + exePath + "\" < \"" + inputPath + "\" > \"" + outputPath + "\" 2>&1";

    auto startTime = std::chrono::high_resolution_clock::now();
    int exitCode = std::system(cmd.c_str());
    auto endTime = std::chrono::high_resolution_clock::now();

    result.timeTakenMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    // 124 is the standard timeout exit code on Linux.
    if (exitCode == 124 || exitCode == 31744) { 
        result.status = RunStatus::TimeLimitExceeded;
        result.actualOutput = "[Time Limit Exceeded]";
        return result;
    }

    if (exitCode != 0) {
        result.status = RunStatus::RuntimeError;
        // Try to read whatever output was produced (e.g. segfault messages)
        std::ifstream outFile(outputPath);
        if (outFile.is_open()) {
            std::stringstream ss;
            ss << outFile.rdbuf();
            result.actualOutput = ss.str();
        }
        if (result.actualOutput.empty()) {
            result.actualOutput = "[Runtime Error — exit code " + std::to_string(exitCode) + "]";
        }
        return result;
    }

    result.status = RunStatus::Passed; // Will be updated after comparison
    return result;
}
#endif

// CONCURRENCY FIX: Added `submissionId` to the arguments here
RunResult runTestCase(const std::string& exePath,
                      const TestCase& testCase,
                      const std::string& tempDir,
                      int timeLimitMs,
                      const std::string& submissionId) {
    
    // Now submissionId is properly passed in and used
    std::string outputPath = (fs::path(tempDir) / ("actual_" + testCase.id + "_" + submissionId + ".txt")).string();

    // Run with timeout
    RunResult result = runWithTimeout(exePath, testCase.inputPath, outputPath, timeLimitMs);
    result.testId = testCase.id;

    // If we got a TLE or RE, return early
    if (result.status == RunStatus::TimeLimitExceeded ||
        result.status == RunStatus::RuntimeError) {
        // Still try to read expected output for the report
        try {
            result.expectedOutput = FileReader::readFile(testCase.expectedOutputPath);
        } catch (...) {}
        
        // CLEANUP: Remove the temp output file before returning
        std::filesystem::remove(outputPath);
        return result;
    }

    // Read actual output
    try {
        std::ifstream outFile(outputPath);
        if (outFile.is_open()) {
            std::stringstream ss;
            ss << outFile.rdbuf();
            result.actualOutput = ss.str();
        }
    } catch (...) {
        result.actualOutput = "";
    }

    // Read expected output
    try {
        result.expectedOutput = FileReader::readFile(testCase.expectedOutputPath);
    } catch (const std::exception& e) {
        result.status = RunStatus::RuntimeError;
        result.actualOutput = "Cannot read expected output: " + std::string(e.what());
        std::filesystem::remove(outputPath);
        return result;
    }

    // Compare
    if (Comparator::compare(result.actualOutput, result.expectedOutput)) {
        result.status = RunStatus::Passed;
    } else {
        result.status = RunStatus::Failed;
    }
    
    // CLEANUP: Remove the temp output file after reading it
    std::filesystem::remove(outputPath);

    return result;
}

} // namespace Runner