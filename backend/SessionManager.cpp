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

// NEW METHOD: Saves the error and resets the hint level if they make a brand new mistake
void SessionManager::updateSessionError(const std::string& userId, const std::string& mistakeCategory) {
    auto& session = sessions_[userId];
    
    if (session.lastMistakeCategory != mistakeCategory) {
        session.hintLevel = 0; // Start at 0 so advanceHint() makes it 1 on the first try
        session.lastMistakeCategory = mistakeCategory;
    }
}