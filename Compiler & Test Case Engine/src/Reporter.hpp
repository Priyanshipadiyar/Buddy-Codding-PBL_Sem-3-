#pragma once
#include <string>
#include <vector>
#include "Runner.hpp"
#include "Compiler.hpp"

namespace Reporter {

// Prints the full test results report to stdout and saves plain text to outputFilePath if provided.
void printReport(const std::string& questionId,
                 const std::vector<RunResult>& results,
                 const std::string& outputFilePath = "");

// Prints a compile error report to stdout and saves plain text to outputFilePath if provided.
void printCompileError(const std::string& questionId,
                       const CompileResult& compileResult,
                       const std::string& outputFilePath = "");

// Prints a general error message.
void printError(const std::string& message);

} // namespace Reporter
