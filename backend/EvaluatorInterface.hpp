#pragma once
#include <string>

// ------------------------------------------------------------------
// INTEGRATION BOUNDARY: GCC COMPILER ADAPTER
// Implement this interface mapping it to your real GCC execution logic.
// ------------------------------------------------------------------
class EvaluatorInterface {
public:
    virtual ~EvaluatorInterface() = default;
    
    // Should compile sourcePath into binaryPath. Return "Success" or compile errors.
    virtual std::string compile(const std::string& sourcePath, const std::string& binaryPath) = 0;
    
    // Should execute binaryPath feeding standard input from inputPath. Return exact standard output.
    virtual std::string run(const std::string& binaryPath, const std::string& inputPath) = 0;
};
