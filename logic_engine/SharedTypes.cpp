#include "SharedTypes.hpp"

MistakeCategory mapCategoryStringToEnum(const std::string& cat) {
    if (cat.find("init") != std::string::npos) return MistakeCategory::IncorrectInitialization;
    if (cat.find("condition") != std::string::npos) return MistakeCategory::WrongCondition;
    if (cat.find("bound") != std::string::npos) return MistakeCategory::BoundaryOrIndexError;
    if (cat.find("loop") != std::string::npos) return MistakeCategory::LoopError;
    if (cat.find("update") != std::string::npos) return MistakeCategory::WrongVariableUpdate;
    if (cat.find("logic") != std::string::npos || cat.find("approach") != std::string::npos) return MistakeCategory::WrongApproach;
    if (cat.find("edge") != std::string::npos || cat.find("not_found") != std::string::npos) return MistakeCategory::MissingEdgeCase;
    return MistakeCategory::Unknown;
}

std::string mapCategoryEnumToString(MistakeCategory cat) {
    switch(cat) {
        case MistakeCategory::IncorrectInitialization: return "incorrect_initialization";
        case MistakeCategory::WrongCondition: return "wrong_condition";
        case MistakeCategory::BoundaryOrIndexError: return "boundary_or_index_error";
        case MistakeCategory::LoopError: return "loop_error";
        case MistakeCategory::WrongVariableUpdate: return "wrong_variable_update";
        case MistakeCategory::WrongApproach: return "wrong_approach";
        case MistakeCategory::MissingEdgeCase: return "missing_edge_case";
        case MistakeCategory::Unknown: return "unknown";
        default: return "none";
    }
}
