#pragma once
#include <string>
#include <vector>

struct TestCase {
    std::string id;
    std::string input_path;
    std::string expected_output_path;
    std::string expected_output_content;
};

struct Problem {
    std::string id;
    std::string statement;
    std::vector<TestCase> test_cases;
};

class DatabaseRepo {
public:
    DatabaseRepo(const std::string& db_dir, const std::string& tc_dir);
    bool loadProblem(const std::string& problemId, Problem& outProblem);
    std::vector<std::string> listProblems();
private:
    std::string db_dir_;
    std::string tc_dir_;
};
