#pragma once
#include <string>
#include <vector>
#include "json.hpp" // nlohmann/json single header

struct DetectRule {
    std::vector<std::string> anyPattern;
    std::vector<std::string> failsAnyOf;
};

struct MistakeRule {
    std::string category;
    int priority;
    DetectRule detect;
    std::string title;
};

class RuleRepository {
public:
    static std::vector<MistakeRule> getRulesForProblem(const std::string& problemId, const std::string& dbPath = "database/problems/");
};