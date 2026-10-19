#include "DatabaseRepo.hpp"
#include "SessionManager.hpp"
#include "Controller.hpp"
#include "TerminalUI.hpp"
#include "CodeCheckerAdapter.hpp" 
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Fixed paths to run correctly when called from your root Node.js server
    DatabaseRepo db("database/problems", "test_cases");
    SessionManager session;
    
    // Instantiate the CodeChecker
    CodeCheckerAdapter codeChecker; 
    
    // Pass it into the controller
    Controller controller(&db, &session, &codeChecker); 

    // ─── MODE 1: NODE.JS HEADLESS EXECUTION ───
    if (argc == 4) {
        std::string userId = argv[1];
        std::string problemId = argv[2];
        std::string sourcePath = argv[3];
        
        SubmissionResult result = controller.submitCode(problemId, sourcePath, userId);
        
        if (result.passed) {
            std::cout << "STATUS: PASS\n";
            std::cout << "MESSAGE: All tests passed successfully.\n";
        } else {
            std::cout << "STATUS: FAIL\n";
            std::cout << "FAILED_TEST: " << result.failedTestCaseName << "\n";
            std::cout << "MISTAKE_CATEGORY: " << result.logicMistakeCategory << "\n";
            std::cout << "PROGRESSIVE_HINT: " << result.progressiveHint << "\n";
            std::cout << "COMPILER_OUTPUT: " << result.compilerOutput << "\n";
        }
        
        return 0; // Exit immediately, do not open TerminalUI
    }

    // ─── MODE 2: LOCAL TERMINAL TESTING ───
    std::cout << "Booting Buddy Coding Integrated Backend (Interactive Local Mode)..." << std::endl;
    
    TerminalUI ui(&controller, &db);
    ui.run();
    
    return 0;
}