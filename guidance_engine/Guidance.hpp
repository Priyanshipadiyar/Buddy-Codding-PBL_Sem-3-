#pragma once
#include <string>

// Retaining existing interface from the zip
struct GuidanceResponse
{
    int hintLevel;
    std::string hintText;
    bool offerEasierProblem;
};

class GuidanceEngine
{
private:
    std::string dbPath;
public:
    GuidanceEngine(const std::string& dbPath = "database/problems/") : dbPath(dbPath) {}
    
    GuidanceResponse getGuidance(
        const std::string& problemId,
        const std::string& mistakeCategory,
        int attemptCount
    );
};