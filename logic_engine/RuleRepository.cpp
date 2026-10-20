#include "RuleRepository.hpp"
#include <fstream>
#include <iostream>
#include <algorithm> 

using json = nlohmann::json;

std::vector<MistakeRule> RuleRepository::getRulesForProblem(const std::string& problemId, const std::string& dbPath) {
    std::vector<MistakeRule> rules;
    std::string filepath = dbPath + problemId + ".json";
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "[LogicEngine] Warning: Could not open rule file " << filepath << "\n";
        return rules;
    }

    try {
        json j;
        file >> j;
        if (j.contains("mistakes")) {
            for (const auto& m : j["mistakes"]) {
                MistakeRule rule;
                rule.category = m.value("category", "unknown");
                rule.priority = m.value("priority", 999);
                rule.title = m.value("title", "No description");
                
                if (m.contains("detect")) {
                    if (m["detect"].contains("anyPattern")) {
                        for (const auto& pat : m["detect"]["anyPattern"]) 
                            rule.detect.anyPattern.push_back(pat);
                    }
                    if (m["detect"].contains("failsAnyOf")) {
                        for (const auto& fail : m["detect"]["failsAnyOf"]) 
                            rule.detect.failsAnyOf.push_back(fail);
                    }
                }
                rules.push_back(rule);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[LogicEngine] JSON Parse Error: " << e.what() << "\n";
    }

    // --- SORTING BLOCK ---
    std::sort(rules.begin(), rules.end(), [](const MistakeRule& a, const MistakeRule& b) {
        return a.priority < b.priority; 
    });
    // ---------------------

    return rules;
}