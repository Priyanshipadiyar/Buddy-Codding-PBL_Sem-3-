
#include "TerminalUI.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <limits>

// Terminal colors
#define RESET   "\033[0m"
#define PURPLE  "\033[1;35m"
#define CYAN    "\033[1;36m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define BOLD    "\033[1m"

// Display the Buddy Coding header
static void printHeader() {
    std::cout << PURPLE << BOLD;
    std::cout << "\n========================================\n";
    std::cout << "             BUDDY CODING\n";
    std::cout << "        Your Coding Companion\n";
    std::cout << "========================================\n";
    std::cout << RESET;
}

// Constructor
TerminalUI::TerminalUI(Controller* controller, DatabaseRepo* db)
    : controller_(controller), db_(db) {}

// Main menu
void TerminalUI::run() {
    while (true) {
        printHeader();

        std::cout << CYAN << "Active Problem: " << RESET
                  << (activeProblemId_.empty()
                          ? "None"
                          : activeProblemId_)
                  << "\n\n";

        std::cout << PURPLE << BOLD;
        std::cout << "  [1] List Available Questions\n";
        std::cout << "  [2] Select Problem & Submit Code\n";
        std::cout << "  [3] Request Next Hint\n";
        std::cout << "  [0] Exit\n";
        std::cout << RESET;

        std::cout << CYAN << "\nChoose an option: " << RESET;

        int choice;

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );
            std::cout << RED << "Invalid input.\n" << RESET;
            continue;
        }

        switch (choice) {
            case 1:
                listQuestions();
                break;

            case 2:
                submitSolution();
                break;

            case 3:
                requestHint();
                break;

            case 0:
                std::cout << GREEN
                          << "\nKeep learning. Keep growing!\n"
                          << RESET;
                return;

            default:
                std::cout << RED
                          << "\nInvalid choice. Please try again.\n"
                          << RESET;
        }
    }
}

// Display available questions
void TerminalUI::listQuestions() {
    auto problems = db_->listProblems();

    std::cout << CYAN << "\n========== AVAILABLE QUESTIONS ==========\n"
              << RESET;

    if (problems.empty()) {
        std::cout << RED
                  << "No questions found. Check your database directory.\n"
                  << RESET;
        return;
    }

    for (size_t i = 0; i < problems.size(); ++i) {
        Problem p;

        if (db_->loadProblem(problems[i], p)) {
            std::cout << YELLOW << "[" << i + 1 << "] "
                      << RESET << GREEN << p.id << RESET
                      << " - " << p.statement << "\n";
        } else {
            std::cout << YELLOW << "[" << i + 1 << "] "
                      << RESET << problems[i] << "\n";
        }
    }

    std::cout << CYAN
              << "=========================================\n"
              << RESET;
}

// Select a problem and submit source code
void TerminalUI::submitSolution() {
    auto problems = db_->listProblems();

    if (problems.empty()) {
        std::cout << RED
                  << "\nNo problems found in the database.\n"
                  << RESET;
        return;
    }

    std::cout << CYAN << "\nAvailable problem IDs:\n" << RESET;

    for (size_t i = 0; i < problems.size(); ++i) {
        std::cout << YELLOW << i + 1 << ". "
                  << RESET << problems[i] << "\n";
    }

    std::cout << CYAN
              << "\nEnter problem number (1-" << problems.size()
              << "): " << RESET;

    int probIndex;

    if (!(std::cin >> probIndex)) {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
        std::cout << RED << "Invalid input.\n" << RESET;
        return;
    }

    if (probIndex < 1 ||
        probIndex > static_cast<int>(problems.size())) {
        std::cout << RED
                  << "Invalid problem number.\n"
                  << RESET;
        return;
    }

    activeProblemId_ = problems[probIndex - 1];

    Problem problem;

    std::cout << GREEN << "\nSelected: "
              << activeProblemId_ << "\n" << RESET;

    if (db_->loadProblem(activeProblemId_, problem)) {
        std::cout << CYAN << "\nProblem Statement:\n"
                  << RESET << problem.statement << "\n";
    }

    std::cout << PURPLE
              << "\nPaste your C++ code below.\n"
              << RESET;

    std::cout << YELLOW
              << "Type END_CODE on a separate line when finished.\n\n"
              << RESET;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    std::string line;
    std::string sourceCode;

    while (std::getline(std::cin, line)) {
        if (line == "END_CODE") {
            break;
        }

        sourceCode += line + "\n";
    }

    if (sourceCode.empty()) {
        std::cout << RED
                  << "\nNo code entered. Submission cancelled.\n"
                  << RESET;
        return;
    }

    const std::string tempPath = "temp_submission.cpp";

    std::ofstream outFile(tempPath);

    if (!outFile) {
        std::cout << RED
                  << "Could not create the submission file.\n"
                  << RESET;
        return;
    }

    outFile << sourceCode;
    outFile.close();

    if (!outFile) {
        std::cout << RED
                  << "Could not write the submission file.\n"
                  << RESET;
        return;
    }

    std::cout << CYAN
              << "\nEvaluating submission...\n"
              << RESET;

    SubmissionResult res =
        controller_->submitCode(
            activeProblemId_,
            tempPath,
            currentUserId_
        );

    if (!res.compiled) {
        std::cout << RED << "\n[COMPILATION FAILED]\n"
                  << RESET;
        std::cout << res.compilerOutput << "\n";
        return;
    }

    if (res.passed) {
        std::cout << GREEN << BOLD
                  << "\n[SUCCESS] All test cases passed!\n"
                  << RESET;
        std::cout << GREEN
                  << "Great work! Your solution is correct.\n"
                  << RESET;
    } else {
        std::cout << YELLOW << BOLD
                  << "\n[NOT QUITE YET]\n"
                  << RESET;

        std::cout << CYAN << "Tests passed: "
                  << RESET << res.testsPassed
                  << "/" << res.testsTotal << "\n";

        if (!res.failedTestCaseName.empty()) {
            std::cout << YELLOW << "Failed test: "
                      << RESET << res.failedTestCaseName << "\n";
        }

        if (!res.progressiveHint.empty()) {
            std::cout << PURPLE << "\nBuddy's Hint: "
                      << RESET << res.progressiveHint << "\n";
        }

        std::cout << CYAN
                  << "\nYou can edit your code and submit again,\n"
                  << "or choose option 3 for another hint.\n"
                  << RESET;
    }
}

// Request the next hint from the existing backend
void TerminalUI::requestHint() {
    if (activeProblemId_.empty()) {
        std::cout << YELLOW
                  << "\nNo active problem selected.\n"
                  << "Choose option 2 to select a problem first.\n"
                  << RESET;
        return;
    }

    std::cout << PURPLE << BOLD
              << "\n========== BUDDY'S GUIDANCE ==========\n"
              << RESET;

    std::cout << CYAN << "Problem: "
              << RESET << activeProblemId_ << "\n";

    std::string hint =
        controller_->getNextHint(
            activeProblemId_,
            currentUserId_
        );

    if (hint.empty()) {
        std::cout << YELLOW
                  << "\nNo additional hint is available right now.\n"
                  << RESET;
    } else {
        std::cout << GREEN << "\nHint:\n"
                  << RESET << hint << "\n";
    }

    std::cout << PURPLE
              << "\nThink through the logic and try again!\n"
              << RESET;
}
