#include "Compiler.hpp"
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <array>
#include <memory>

namespace fs = std::filesystem;

namespace Compiler {

// Update the signature in both Compiler.hpp and Compiler.cpp
CompileResult compile(const std::string& sourceCode, const std::string& tempDir, const std::string& submissionId) {
    CompileResult result;

    // Ensure temp directory exists
    fs::create_directories(tempDir);

    // Append the unique ID to prevent overlapping files
    std::string srcPath = (fs::path(tempDir) / ("student_code_" + submissionId + ".cpp")).string();
    std::string exePath = (fs::path(tempDir) / ("student_code_" + submissionId + ".exe")).string();
    std::string errPath = (fs::path(tempDir) / ("compile_errors_" + submissionId + ".txt")).string();

    {
        std::ofstream srcFile(srcPath);
        if (!srcFile.is_open()) {
            result.success = false;
            result.errorOutput = "Internal error: cannot write to " + srcPath;
            return result;
        }
        srcFile << sourceCode;
    }

    // Build g++ command — redirect stderr to a file so we can capture it
    std::string command = "g++ -std=c++17 -o \"" + exePath + "\" \"" + srcPath + "\" 2> \"" + errPath + "\"";

    int exitCode = std::system(command.c_str());

    // Read any compiler errors/warnings
    std::ifstream errFile(errPath);
    if (errFile.is_open()) {
        std::string line;
        while (std::getline(errFile, line)) {
            result.errorOutput += line + "\n";
        }
    }

    result.success = (exitCode == 0);
    // Optionally clean up the source and error files here after reading them
    return result;
}}
