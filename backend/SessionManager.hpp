#pragma once
#include <string>
#include <map>

struct Session {
    std::string currentProblemId;
    int attempts = 0;
    int hintLevel = 0; 
    bool solved = false;
    
    // NEW: The Controller needs to store the last mistake here
    std::string lastMistakeCategory = "none";
    std::string lastMistakeKey;   // "problemId:category", used to detect a new mistake
};

class SessionManager {
public:
    Session& getSession(const std::string& userId);
    void recordAttempt(const std::string& userId, bool passed);
    int advanceHint(const std::string& userId);
    
    // NEW: Method for the Controller to update the mistake state
    void updateSessionError(const std::string& userId, const std::string& mistakeCategory,
                            const std::string& problemId = "");

private:
    std::map<std::string, Session> sessions_;
};