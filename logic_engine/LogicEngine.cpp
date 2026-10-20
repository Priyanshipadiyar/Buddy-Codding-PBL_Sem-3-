#include "LogicEngine.hpp"
#include "RuleRepository.hpp"
#include <regex>
#include <algorithm>
#include <iostream>

LogicResult LogicEngine::evaluate(const std::string& problemId, 
                                  const std::string& sourceCode, 
                                  const std::vector<std::string>& failedTestIds) {
    LogicResult result;
    result.problemId = problemId;
    result.classified = false;
    result.categoryEnum = MistakeCategory::Unknown;
    result.categoryString = "unknown";
    result.priority = 9999;
    
    if (failedTestIds.empty()) {
        result.explanation = "No failed tests provided to Logic Engine.";
        return result;
    }

    std::vector<MistakeRule> rules = RuleRepository::getRulesForProblem(problemId, dbPath);
    if (rules.empty()) {
        result.explanation = "No rules found for problem.";
        return result;
    }

    std::vector<MistakeRule> firedRules;

    for (const auto& rule : rules) {
        // Condition A: Did a required test fail?
        bool testFailed = false;
        if (rule.detect.failsAnyOf.empty()) {
            testFailed = true; // If no test condition, assume it can trigger
        } else {
            for (const auto& requiredFail : rule.detect.failsAnyOf) {
                if (std::find(failedTestIds.begin(), failedTestIds.end(), requiredFail) != failedTestIds.end()) {
                    testFailed = true;
                    break;
                }
            }
        }

        if (!testFailed) continue;

        // Condition B: Does the regex match?
        bool patternMatched = false;
        if (rule.detect.anyPattern.empty() || (rule.detect.anyPattern.size()==1 && rule.detect.anyPattern[0].empty())) {
            patternMatched = true; // No pattern required
        } else {
            for (const auto& patStr : rule.detect.anyPattern) {
                try {
                    std::regex pattern(patStr);
                    if (std::regex_search(sourceCode, pattern)) {
                        patternMatched = true;
                        break;
                    }
                } catch(const std::regex_error& e) {
                    std::cerr << "[LogicEngine] Invalid regex in rules: " << patStr << " (" << e.what() << ")\n";
                }
            }
        }

        if (testFailed && patternMatched) {
            firedRules.push_back(rule);
        }
    }

    if (firedRules.empty()) {
        result.explanation = "Tests failed, but no predefined logical patterns matched.";
        return result;
    }

    // Pick lowest priority number
    auto bestRule = std::min_element(firedRules.begin(), firedRules.end(), [](const MistakeRule& a, const MistakeRule& b){
        return a.priority < b.priority;
    });

    result.classified = true;
    result.categoryString = bestRule->category;
    result.categoryEnum = mapCategoryStringToEnum(bestRule->category);
    result.priority = bestRule->priority;
    result.explanation = "Detected mistake: " + bestRule->title;

    return result;
}
