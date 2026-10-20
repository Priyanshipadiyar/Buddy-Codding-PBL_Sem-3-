#pragma once
#include "SharedTypes.hpp"
#include <string>
#include <vector>

class LogicEngine {
private:
    std::string dbPath;
public:
    LogicEngine(const std::string& dbPath = "../database/problems/") : dbPath(dbPath) {}
    
    // Analyzes the code and failed tests against predefined rules
    LogicResult evaluate(const std::string& problemId, 
                         const std::string& sourceCode, 
                         const std::vector<std::string>& failedTestIds);
};
