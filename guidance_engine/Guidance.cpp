#include "Guidance.hpp"

GuidanceResponse GuidanceEngine::getGuidance(
    const std::string& problemId,
    const std::string& mistakeCategory,
    int attemptCount)
{
    GuidanceResponse response;

    // Hint level
    if (attemptCount <= 1)
    {
        response.hintLevel = 1;
    }
    else
    {
        response.hintLevel = 2;
    }

    // Hint according to Member 2's mistake categories
    if (mistakeCategory == "incorrect_initialization")
    {
        if (response.hintLevel == 1)
        {
            response.hintText =
                "Check the initial value given to the variable.";
        }
        else
        {
            response.hintText =
                "Check whether the variable has been initialized with the correct value.";
        }
    }
    else if (mistakeCategory == "wrong_condition")
    {
        if (response.hintLevel == 1)
        {
            response.hintText =
                "Think about whether the condition used in your logic is correct.";
        }
        else
        {
            response.hintText =
                "Check the condition carefully according to the problem requirements.";
        }
    }
    else if (mistakeCategory == "loop_boundary_error")
    {
        if (response.hintLevel == 1)
        {
            response.hintText =
                "Think about where the loop should start and stop.";
        }
        else
        {
            response.hintText =
                "Check the loop boundary and make sure all required elements are processed.";
        }
    }
    else
    {
        response.hintText =
            "Review the logic related to the identified mistake.";
    }

    // Offer easier problem after 2 or more attempts
    if (attemptCount >= 2)
    {
        response.offerEasierProblem = true;
    }
    else
    {
        response.offerEasierProblem = false;
    }

    return response;
}