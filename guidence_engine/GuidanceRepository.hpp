#pragma once
#include <string>
#include <map>
#include "json.hpp"

struct GuidanceData {
    std::string hint1;
    std::string hint2;
    std::string hint3;
    std::string hint4;
};

class GuidanceRepository {
public:
    static GuidanceData getGuidanceForMistake(const std::string& problemId, const std::string& mistakeCategory, const std::string& dbPath = "database/problems/");
};