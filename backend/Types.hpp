#pragma once
#include <string>
#include <vector>

// The kinds of "wrong reasoning" the system knows about.
// Names are shared by the whole team - do not rename without telling everyone.
enum class MistakeCategory {
    None,
    IncorrectInitialization,
    WrongCondition,
    BoundaryOrIndexError,
    LoopError,
    WrongVariableUpdate,
    WrongApproach,
    MissingEdgeCase,
    Unknown
};

// The overall outcome of one submission.
enum class Status {
    Accepted,
    WrongAnswer,
    CompileError,
    RuntimeError,
    InvalidSubmission   // empty / unusable input
};

// One question in the question bank.
struct Problem {
    int id;
    std::string title;
    std::string statement;
    std::string topic;       // e.g. "Arrays"
    int difficulty;          // 1 = easy ... 3 = hard
};

// What the evaluator hands back after checking an attempt.
struct EvaluationResult {
    Status status = Status::InvalidSubmission;
    MistakeCategory mistake = MistakeCategory::None;
    std::string message;     // short text for the student
};

// Helper functions: turn enums into readable text.
std::string toString(Status s);
std::string toString(MistakeCategory m);
