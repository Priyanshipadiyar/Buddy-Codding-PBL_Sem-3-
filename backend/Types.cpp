#include "Types.hpp"

std::string toString(Status s) {
    switch (s) {
        case Status::Accepted:          return "accepted";
        case Status::WrongAnswer:       return "wrong_answer";
        case Status::CompileError:      return "compile_error";
        case Status::RuntimeError:      return "runtime_error";
        case Status::InvalidSubmission: return "invalid_submission";
    }
    return "unknown";
}

std::string toString(MistakeCategory m) {
    switch (m) {
        case MistakeCategory::None:                    return "none";
        case MistakeCategory::IncorrectInitialization: return "incorrect_initialization";
        case MistakeCategory::WrongCondition:          return "wrong_condition";
        case MistakeCategory::BoundaryOrIndexError:    return "boundary_or_index_error";
        case MistakeCategory::LoopError:               return "loop_error";
        case MistakeCategory::WrongVariableUpdate:     return "wrong_variable_update";
        case MistakeCategory::WrongApproach:           return "wrong_approach";
        case MistakeCategory::MissingEdgeCase:         return "missing_edge_case";
        case MistakeCategory::Unknown:               return "unknown";
    }
    return "unknown";
}
