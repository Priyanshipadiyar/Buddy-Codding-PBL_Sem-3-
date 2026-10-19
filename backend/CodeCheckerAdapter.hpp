#pragma once
#include "EvaluatorInterface.hpp"
#include <string>

class CodeCheckerAdapter : public EvaluatorInterface {
public:
    // Implements the compile requirement
    std::string compile(const std::string& sourcePath, const std::string& binaryPath) override;
    
    // Implements the run/execute requirement
    std::string run(const std::string& binaryPath, const std::string& inputPath) override;
};