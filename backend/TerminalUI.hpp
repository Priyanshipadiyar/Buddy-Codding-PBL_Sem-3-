#pragma once
#include "Controller.hpp"

class TerminalUI {
public:
    TerminalUI(Controller* controller, DatabaseRepo* db);
    void run();
private:
    void listQuestions();
    void submitSolution();
    void requestHint();

    Controller* controller_;
    DatabaseRepo* db_;
    std::string currentUserId_ = "terminal_student";
    std::string activeProblemId_ = "";
};
