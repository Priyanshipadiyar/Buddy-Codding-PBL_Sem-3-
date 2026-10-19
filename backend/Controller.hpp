#pragma once
#include <string>
#include "DatabaseRepo.hpp"
#include "SessionManager.hpp"
#include "EvaluatorInterface.hpp" 

// Forward declarations for existing engines to prevent header loops
class LogicEngine;
class GuidanceRepository;

struct SubmissionResult {
    bool compiled = false;
    bool passed = false;
    int testsTotal = 0;
    int testsPassed = 0;
    std::string compilerOutput;
    std::string failedTestCaseName;
    std::string expectedOutput;
    std::string actualOutput;
    std::string logicMistakeCategory;
    std::string progressiveHint;
};

class Controller {
public:
    Controller(DatabaseRepo* db, SessionManager* session, EvaluatorInterface* evaluator);
    
    SubmissionResult submitCode(const std::string& problemId, const std::string& sourceCodePath, const std::string& userId);
    std::string getNextHint(const std::string& problemId, const std::string& userId);

private:
    DatabaseRepo* db_;
    SessionManager* session_;
    EvaluatorInterface* evaluator_;
    
    // Pointers mapped to your actual engines
    LogicEngine* logicEngine_ = nullptr;
    GuidanceRepository* guidanceRepo_ = nullptr;
};
