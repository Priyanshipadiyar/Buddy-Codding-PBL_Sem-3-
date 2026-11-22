#include "SessionManager.hpp"
#include <algorithm> // Required for std::min

Session& SessionManager::getSession(const std::string& userId) {
    return sessions_[userId];
}

void SessionManager::recordAttempt(const std::string& userId, bool passed) {
    auto& session = sessions_[userId];
    session.attempts++;
    if (passed) {
        session.solved = true;
    }
}

int SessionManager::advanceHint(const std::string& userId) {
    auto& session = sessions_[userId];
    
    // Cap the hint level at 4 so it doesn't try to fetch hint5, hint6, etc.
    session.hintLevel = std::min(session.hintLevel + 1, 4);
    
    return session.hintLevel;
}

// Saves the mistake and resets the hint level when it is a new mistake
// (different category OR different problem).
void SessionManager::updateSessionError(const std::string& userId,
                                        const std::string& mistakeCategory,
                                        const std::string& problemId) {
    auto& session = sessions_[userId];
    std::string key = problemId + ":" + mistakeCategory;

    if (session.lastMistakeKey != key) {
        session.hintLevel = 0;  // advanceHint() will make it 1
        session.lastMistakeKey = key;
    }
    session.lastMistakeCategory = mistakeCategory;
    session.currentProblemId = problemId;
}