#include "Compiler.hpp"
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <array>
#include <memory>

namespace fs = std::filesystem;

namespace Compiler {

CompileResult compile(const std::string& sourceCode, const std::string& tempDir) {
    CompileResult result;

    // Ensure temp directory exists
    fs::create_directories(tempDir);

    // Write source code to temp file
    std::string srcPath = (fs::path(tempDir) / "student_code.cpp").string();
    std::string exePath = (fs::path(tempDir) / "student_code.exe").string();
    std::string errPath = (fs::path(tempDir) / "compile_errors.txt").string();

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
    return result;
}

} // namespace Compiler
