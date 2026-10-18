#pragma once
#include <string>

struct CompileResult {
    bool success;
    std::string errorOutput;
};

namespace Compiler {

// Writes sourceCode to temp/student_code.cpp, then compiles it with g++.
// Returns CompileResult with success status and any error output.
CompileResult compile(const std::string& sourceCode, const std::string& tempDir);

} // namespace Compiler
