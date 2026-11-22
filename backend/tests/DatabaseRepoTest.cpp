#include "DatabaseRepo.hpp"
#include <iostream>

int main() {
    DatabaseRepo repo("database/problems", "test_cases");
    const auto ids = repo.listProblems();
    if (ids.size() != 35) {
        std::cerr << "Expected 35 problem JSON files, found " << ids.size() << '\n';
        return 1;
    }
    for (const auto& id : ids) {
        Problem problem;
        if (!repo.loadProblem(id, problem)) {
            std::cerr << "Could not load " << id << '\n';
            return 1;
        }
        if (problem.test_cases.empty()) {
            std::cerr << id << " has no valid test cases\n";
            return 1;
        }
        if (problem.title.empty() || problem.statement.empty()) {
            std::cerr << id << " is missing its title or statement\n";
            return 1;
        }
    }
    Problem missing;
    if (repo.loadProblem("Q999", missing)) {
        std::cerr << "Unknown problem ID should not load\n";
        return 1;
    }
    std::cout << "DatabaseRepo test passed: " << ids.size()
              << " problems loaded with valid test cases.\n";
    return 0;
}
