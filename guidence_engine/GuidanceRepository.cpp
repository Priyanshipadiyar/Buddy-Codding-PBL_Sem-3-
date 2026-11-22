#include "GuidanceRepository.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

GuidanceData GuidanceRepository::getGuidanceForMistake(const std::string& problemId, const std::string& mistakeCategory, const std::string& dbPath) {
    GuidanceData data;
    std::string filepath = dbPath + problemId + ".json";
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "[GuidanceEngine] Missing file: " << filepath << "\n";
        return data;
    }

    try {
        json j;
        file >> j;
        if (j.contains("mistakes")) {
            for (const auto& m : j["mistakes"]) {
                if (m.value("category", "") == mistakeCategory) {
                    data.hint1 = m.value("hint1", "");
                    data.hint2 = m.value("hint2", "");
                    data.hint3 = m.value("hint3", "");
                    data.hint4 = m.value("hint4", "");
                    break;
                }
            }
        }
    } catch (...) {}

    return data;
}
