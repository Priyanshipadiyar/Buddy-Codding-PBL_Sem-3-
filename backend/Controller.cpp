
#include "Controller.hpp"
#include <iostream>
#include <filesystem>
#include <regex>

// --- INTEGRATION INCLUDES ---
#include "../logic_engine/RuleRepository.hpp"
#include "../guidence_engine/GuidanceRepository.hpp"
// ----------------------------

Controller::Controller(
    DatabaseRepo* db,
    SessionManager* session,
    EvaluatorInterface* evaluator
)
    : db_(db), session_(session), evaluator_(evaluator) {}


SubmissionResult Controller::submitCode(
    const std::string& problemId,
    const std::string& sourceCodePath,
    const std::string& userId
) {
    SubmissionResult result;
    Problem problem;

    if (!db_->loadProblem(problemId, problem)) {
        result.compilerOutput = "Error: Problem ID not found.";
        return result;
    }

    if (!evaluator_) {
        result.compilerOutput =
            "SYSTEM BLOCKER: Real GCC Evaluator is not yet connected. "
            "Submission evaluation unavailable.";
        return result;
    }

    // Unique binary path for each user.
    std::string binaryPath = "temp_student_bin_" + userId;

    std::string compileMsg =
        evaluator_->compile(sourceCodePath, binaryPath);

    if (compileMsg != "Success") {
        result.compiled = false;
        result.compilerOutput = compileMsg;

        session_->recordAttempt(userId, false);

        // Clean up source file after compilation failure.
        std::filesystem::remove(sourceCodePath);

        return result;
    }

    result.compiled = true;
    result.testsTotal = problem.test_cases.size();

    for (const auto& tc : problem.test_cases) {

        std::string actualOutput =
            evaluator_->run(binaryPath, tc.input_path);

        // Remove trailing newline and carriage-return characters.
        while (!actualOutput.empty() &&
               (actualOutput.back() == '\n' ||
                actualOutput.back() == '\r')) {
            actualOutput.pop_back();
        }

        if (actualOutput != tc.expected_output_content) {

            result.passed = false;
            result.failedTestCaseName = tc.id;
            result.expectedOutput = tc.expected_output_content;
            result.actualOutput = actualOutput;

// --- 1. LOGIC ENGINE: FETCH RULES ---
std::vector<MistakeRule> rules =
    RuleRepository::getRulesForProblem(problemId);

result.logicMistakeCategory = "unclassified_error";
int bestPriority = 999999;

// --- 2. LOGIC ENGINE: MATCH TEST + CODE PATTERN ---
for (const auto& rule : rules) {

    // Check whether this rule applies to the failed test.
    bool testMatches = rule.detect.failsAnyOf.empty();

    for (const std::string& failedTest :
         rule.detect.failsAnyOf) {

        if (failedTest == tc.id ||
            failedTest == "ALL_TESTS") {
            testMatches = true;
            break;
        }
    }

    if (!testMatches) {
        continue;
    }

    // Empty patterns must NOT match every submission.
    bool hasUsablePattern = false;
    bool patternMatches = false;

    for (const std::string& pattern :
         rule.detect.anyPattern) {

        if (pattern.empty()) {
            continue;
        }

        hasUsablePattern = true;

        try {
            if (std::regex_search(sourceCode, std::regex(pattern))) {
                patternMatches = true;
                break;
            }
        } catch (const std::regex_error&) {
            std::cerr << "Invalid regex for mistake category: "
                      << rule.category << '\n';
        }
    }

    // No meaningful pattern means we cannot identify this mistake.
    if (!hasUsablePattern || !patternMatches) {
        continue;
    }

    // Choose the matching rule with the highest priority
    // (the lowest numeric priority value).
    if (rule.priority < bestPriority) {
        bestPriority = rule.priority;
        result.logicMistakeCategory = rule.category;
    }
}

            // --- 3. GUIDANCE ENGINE: ADVANCE HINTS ---

            // Read the previous mistake BEFORE updating the session.
            
                 std::string previousMistake =
                session_->getSession(userId).lastMistakeCategory;

            bool sameMistake =
                (!previousMistake.empty() &&
                 previousMistake == result.logicMistakeCategory);

            // Save the current mistake.
            session_->updateSessionError(
                userId,
                result.logicMistakeCategory
            );

            int currentHintLevel;

            if (sameMistake) {
                // Repeated mistake: advance to the next hint.
                currentHintLevel = session_->advanceHint(userId);
            } else {
                // New mistake: advance from the reset level (0) to 1.
                currentHintLevel = session_->advanceHint(userId);
            }

            // Retrieve the hints for the detected mistake.
            GuidanceData allHints =
                GuidanceRepository::getGuidanceForMistake(
                    problemId,
                    result.logicMistakeCategory
                );

            // Select the hint matching the current level.
            if (currentHintLevel <= 1) {
                result.progressiveHint = allHints.hint1;
            } else if (currentHintLevel == 2) {
                result.progressiveHint = allHints.hint2;
            } else if (currentHintLevel == 3) {
                result.progressiveHint = allHints.hint3;
            } else {
                result.progressiveHint = allHints.hint4;
            }

            session_->recordAttempt(userId, false);

            // Clean up generated files before returning.
            std::filesystem::remove(binaryPath);
            std::filesystem::remove(sourceCodePath);

            return result;

        } else {
            result.testsPassed++;
        }
    }

    // --- ALL TESTS PASSED ---
    result.passed = true;
    result.compilerOutput = "All tests passed successfully!";

    session_->recordAttempt(userId, true);

    // Clean up generated files after successful execution.
    std::filesystem::remove(binaryPath);
    std::filesystem::remove(sourceCodePath);

    return result;
}


// --- GET NEXT HINT ---
std::string Controller::getNextHint(
    const std::string& problemId,
    const std::string& userId
) {
    // Advance the hint level.
    int newLevel = session_->advanceHint(userId);

    std::string currentMistake =
        session_->getSession(userId).lastMistakeCategory;

    GuidanceData allHints =
        GuidanceRepository::getGuidanceForMistake(
            problemId,
            currentMistake
        );

    if (newLevel <= 1) return allHints.hint1;
    if (newLevel == 2) return allHints.hint2;
    if (newLevel == 3) return allHints.hint3;

    return allHints.hint4;
}
