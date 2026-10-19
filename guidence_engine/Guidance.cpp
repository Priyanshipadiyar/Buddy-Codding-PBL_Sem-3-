#include "Guidance.hpp"
#include "GuidanceRepository.hpp"

GuidanceResponse GuidanceEngine::getGuidance(
        const std::string& problemId,
        const std::string& mistakeCategory,
        int attemptCount) 
{
    GuidanceResponse response;
    response.offerEasierProblem = false;
    
    if (attemptCount <= 0) attemptCount = 1;

    // Fetch from JSON repository
    GuidanceData data = GuidanceRepository::getGuidanceForMistake(problemId, mistakeCategory, dbPath);

    // Fallback if no matching category found
    if (data.hint1.empty()) {
        response.hintLevel = 1;
        response.hintText = "Check your code carefully. Some test cases are failing.";
        return response;
    }

    // Progression Logic
    if (attemptCount == 1) {
        response.hintLevel = 1;
        response.hintText = data.hint1;
    } 
    else if (attemptCount == 2) {
        response.hintLevel = 2;
        response.hintText = data.hint2.empty() ? data.hint1 : data.hint2;
    } 
    else if (attemptCount == 3) {
        response.hintLevel = 3;
        response.hintText = data.hint3.empty() ? (data.hint2.empty() ? data.hint1 : data.hint2) : data.hint3;
        response.offerEasierProblem = true;
    }
    else {
        response.hintLevel = 4;
        response.hintText = data.hint4.empty() ? (data.hint3.empty() ? data.hint2 : data.hint3) : data.hint4;
        if (response.hintText.empty()) response.hintText = "No further hints available. Try asking your instructor.";
        response.offerEasierProblem = true;
    }

    return response;
}
