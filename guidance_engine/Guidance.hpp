#pragma once
#include <string>

struct GuidanceResponse
{
    int hintLevel;
    std::string hintText;
    bool offerEasierProblem;
};

class GuidanceEngine
{
public:
    GuidanceResponse getGuidance(
        const std::string& problemId,
        const std::string& mistakeCategory,
        int attemptCount
    );
};