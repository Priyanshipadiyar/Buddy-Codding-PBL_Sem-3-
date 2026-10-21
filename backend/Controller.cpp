#include "Controller.hpp"
#include <iostream>
#include <filesystem>
#include <regex>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

// --- INTEGRATION INCLUDES ---
#include "../logic_engine/RuleRepository.hpp"
#include "../guidence_engine/GuidanceRepository.hpp"
// ----------------------------

// ---------- Pattern matching helpers ----------

// Removes all whitespace so "nums[n-1] * nums[n-2]" matches "nums[n-1]*nums[n-2]".
static std::string squash(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if (!std::isspace(static_cast<unsigned char>(c))) out += c;
    }
    return out;
}

// Patterns are LITERAL code snippets (whitespace ignored) by default.
// A pattern starting with "re:" is treated as a real regex.
//   "nums[0] * nums[1]"          -> literal match
//   "re:int\\s+max\\w*\\s*=\\s*0" -> regex match
static bool matchesPattern(const std::string& code,
                           const std::string& codeSquashed,
                           const std::string& pattern) {
    if (pattern.rfind("re:", 0) == 0) {
        try {
            return std::regex_search(code, std::regex(pattern.substr(3)));
        } catch (const std::regex_error&) {
            std::cerr << "Invalid regex pattern: " << pattern << '\n';
            return false;
        }
    }
    std::string p = squash(pattern);
    if (p.empty()) return false;
    return codeSquashed.find(p) != std::string::npos;
}

// Strips // and /* */ comments without touching string literals.
static std::string stripComments(const std::string& src) {
    std::string out;
    out.reserve(src.size());
    size_t i = 0, n = src.size();
    while (i < n) {
        if (src[i] == '"' || src[i] == '\'') {          // string / char literal
            char quote = src[i];
            out += src[i++];
            while (i < n && src[i] != quote) {
                if (src[i] == '\\' && i + 1 < n) out += src[i++];
                out += src[i++];
            }
            if (i < n) out += src[i++];
        } else if (src[i] == '/' && i + 1 < n && src[i + 1] == '/') {
            while (i < n && src[i] != '\n') i++;
        } else if (src[i] == '/' && i + 1 < n && src[i + 1] == '*') {
            i += 2;
            while (i + 1 < n && !(src[i] == '*' && src[i + 1] == '/')) i++;
            i = (i + 1 < n) ? i + 2 : n;
            out += ' ';
        } else {
            out += src[i++];
        }
    }
    return out;
}

// ---------- Controller ----------

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

        std::filesystem::remove(sourceCodePath);
        return result;
    }

    result.compiled = true;
    result.testsTotal = problem.test_cases.size();

    // Read the source and strip comments so patterns only see real code.
    std::string sourceCodeContent;
    {
        std::ifstream codeFile(sourceCodePath);
        if (codeFile.is_open()) {
            std::string fileContent((std::istreambuf_iterator<char>(codeFile)),
                                    std::istreambuf_iterator<char>());
            sourceCodeContent = stripComments(fileContent);
        }
    }
    const std::string sourceSquashed = squash(sourceCodeContent);

    // Run every test and collect ALL failures.
    std::vector<std::string> failedTestIds;
    int passedCount = 0;

    for (const auto& tc : problem.test_cases) {
        std::string actualOutput =
            evaluator_->run(binaryPath, tc.input_path);

        while (!actualOutput.empty() &&
               (actualOutput.back() == '\n' ||
                actualOutput.back() == '\r')) {
            actualOutput.pop_back();
        }

        if (actualOutput != tc.expected_output_content) {
            failedTestIds.push_back(tc.id);
            if (result.failedTestCaseName.empty()) {
                result.failedTestCaseName = tc.id;
                result.expectedOutput = tc.expected_output_content;
                result.actualOutput = actualOutput;
            }
        } else {
            passedCount++;
        }
    }

    result.testsPassed = passedCount;
    result.passed = failedTestIds.empty();

    if (!result.passed) {
        // --- 1. LOGIC ENGINE: FETCH RULES ---
        std::vector<MistakeRule> rules =
            RuleRepository::getRulesForProblem(problemId);

        result.logicMistakeCategory = "unclassified_error";
        int bestPriority = 999999;

        // --- 2. LOGIC ENGINE: MATCH TEST + CODE PATTERN ---
        for (const auto& rule : rules) {

            bool testMatches = rule.detect.failsAnyOf.empty();
            for (const std::string& needed : rule.detect.failsAnyOf) {
                if (needed == "ALL_TESTS") { testMatches = true; break; }
                for (const auto& fId : failedTestIds) {
                    if (needed == fId) { testMatches = true; break; }
                }
                if (testMatches) break;
            }
            if (!testMatches) continue;

            bool patternMatches = false;
            for (const std::string& pattern : rule.detect.anyPattern) {
                if (pattern.empty()) continue;
                if (matchesPattern(sourceCodeContent, sourceSquashed, pattern)) {
                    patternMatches = true;
                    break;
                }
            }
            if (!patternMatches) continue;

            // Lowest priority number wins.
            if (rule.priority < bestPriority) {
                bestPriority = rule.priority;
                result.logicMistakeCategory = rule.category;
            }
        }

        // --- 3. GUIDANCE ENGINE: ADVANCE HINTS ---
        // problemId is passed so the hint level resets when the student
        // moves to another problem, even if the category name is the same.
        session_->updateSessionError(
            userId,
            result.logicMistakeCategory,
            problemId
        );

        int currentHintLevel = session_->advanceHint(userId);

        GuidanceData allHints =
            GuidanceRepository::getGuidanceForMistake(
                problemId,
                result.logicMistakeCategory
            );

        if (currentHintLevel <= 1)      result.progressiveHint = allHints.hint1;
        else if (currentHintLevel == 2) result.progressiveHint = allHints.hint2;
        else if (currentHintLevel == 3) result.progressiveHint = allHints.hint3;
        else                            result.progressiveHint = allHints.hint4;

        if (result.progressiveHint.empty()) {
            result.progressiveHint =
                "Compare your output with the expected output on failed test case: "
                + failedTestIds[0];
        }

        session_->recordAttempt(userId, false);

        std::filesystem::remove(binaryPath);
        std::filesystem::remove(sourceCodePath);
        return result;
    }

    // --- ALL TESTS PASSED ---
    result.compilerOutput = "All tests passed successfully!";
    session_->recordAttempt(userId, true);

    std::filesystem::remove(binaryPath);
    std::filesystem::remove(sourceCodePath);
    return result;
}


// --- GET NEXT HINT ---
std::string Controller::getNextHint(
    const std::string& problemId,
    const std::string& userId
) {
    int newLevel = session_->advanceHint(userId);

    std::string currentMistake =
        session_->getSession(userId).lastMistakeCategory;

    GuidanceData allHints =
        GuidanceRepository::getGuidanceForMistake(
            problemId,
            currentMistake
        );

    std::string hint;
    if (newLevel <= 1)      hint = allHints.hint1;
    else if (newLevel == 2) hint = allHints.hint2;
    else if (newLevel == 3) hint = allHints.hint3;
    else                    hint = allHints.hint4;

    if (hint.empty()) {
        hint = "Review your code logic and edge cases.";
    }
    return hint;
}