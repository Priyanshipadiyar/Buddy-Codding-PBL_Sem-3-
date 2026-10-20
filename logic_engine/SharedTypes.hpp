#include "../backend/Types.hpp"
#pragma once
#include <string>
#include <vector>
#include <iostream>

// Re-using the enum from backend/Types.hpp to ensure compatibility


// Extended evaluation result for the Logic Engine
struct LogicResult {
    std::string problemId;
    MistakeCategory categoryEnum;
    std::string categoryString; // Raw category from JSON
    std::string explanation;
    int priority;
    bool classified;
};

// Helper for mapping JSON strings to the existing enum
MistakeCategory mapCategoryStringToEnum(const std::string& cat);
std::string mapCategoryEnumToString(MistakeCategory cat);
